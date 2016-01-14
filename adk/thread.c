#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "debug.h"
#include "thread.h"

typedef struct
{
  void (*task)(void *);
  void *args;
} thread_pool_task_t;

struct thread_pool_t
{
  pthread_mutex_t *mutex;
  pthread_cond_t *cond;
  pthread_t *threads;
  thread_pool_task_t *queue;
  int size;
  int capacity;
  int head;
  int tail;
  thread_pool_state state;
};

static void *pool_thread(void *);

static inline void pool_queue_init(thread_pool_t *pool, int queue_count)
{
  pool->queue =(thread_pool_task_t *) malloc(sizeof(thread_pool_task_t) * queue_count);
  pool->capacity = queue_count;
  pool->head = -1;
  pool->tail = -1;
}

static inline bool pool_queue_is_empty(thread_pool_t *pool)
{
  return pool->head == pool->tail;
}

static inline bool pool_queue_is_full(thread_pool_t *pool)
{
  return (pool->tail - pool->capacity) == pool->head;
}

static inline void pool_queue_cleanup(thread_pool_t *pool)
{
  pool->head = -1;
  pool->tail = -1;
}

static inline bool pool_queue_enqueue(thread_pool_t *pool, void (*task)(void *), void *args )
{
  if (pool_queue_is_full(pool)) return false;
  pool->tail += 1;
  pool->queue[pool->tail % pool->capacity].task = task;
  pool->queue[pool->tail % pool->capacity].args = args;
  return true;
}

static inline thread_pool_task_t *pool_queue_dequeue(thread_pool_t *pool)
{
  if (pool_queue_is_empty(pool)) return NULL;
  pool->head += 1;
  return &(pool->queue[pool->head % pool->capacity]);
}

thread_pool_t *thread_pool_create(int thread_size, int queue_count)
{
  if (thread_size < 1 || queue_count < 1) return NULL;
  thread_pool_t *pool = (thread_pool_t *) malloc(sizeof(thread_pool_t));
  check(pool != NULL, "Can not create a thread pool.");
  pool->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
  pool->cond = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
  pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_size);
  pool->size = 0;
  pool_queue_init(pool, queue_count);

  check(!pthread_mutex_init(pool->mutex, NULL)
        && !pthread_cond_init(pool->cond, NULL)
        && pool->threads != NULL
        && pool->queue != NULL,
        "Thread Pool Initiallization is failed.");

  pool->state = RUNNING;
  for (int i = 0; i < thread_size; i += 1)
  {
    check(!pthread_create(pool->threads + i, NULL, pool_thread, pool)
          , "Can not create a thread in the thread pool.");
    pool->size++;
  }
  return pool;
error:
  if (pool != NULL) thread_pool_destroy(pool);
  return NULL;
}

int thread_pool_add_task(thread_pool_t *pool, void (*task)(void *), void *args)
{
  if (task == NULL && args == NULL) return EINVAL;
  int ret;
  do
  {
    ret = pthread_mutex_lock(pool->mutex);
    if (ret != 0) break;
    pool_queue_enqueue(pool, task, args);
    pthread_cond_signal(pool->cond);
    ret = pthread_mutex_unlock(pool->mutex);
  }
  while (0);
  return ret;
}

int thread_pool_destroy(thread_pool_t *pool)
{
  int ret = 0;
  do
  {
    if (pool == NULL)
    {
      ret = -1;
      break;
    }
    if (pthread_mutex_lock(pool->mutex) != 0)
    {
      ret = -1;
      break;
    }
    pool->state = HALT;
    pool_queue_cleanup(pool);
    /* 删除互斥锁和条件变量 */
    pthread_cond_destroy(pool->cond);
    pthread_mutex_destroy(pool->mutex);

    free(pool->mutex);
    free(pool->cond);
    free(pool->queue);
    free(pool->threads);
    free(pool);
  }
  while (0);
  return ret;
}

static void *pool_thread(void *thread_pool)
{
  thread_pool_t *pool = (thread_pool_t *)thread_pool;
  thread_pool_task_t *task;
  do
  {
    if (pool == NULL || pool->state == HALT) break;
    if (pthread_mutex_lock(pool->mutex) == 0)
    {
      if (pool_queue_is_empty(pool)) pthread_cond_wait(pool->cond, pool->mutex);
      task = pool_queue_dequeue(pool);
      pthread_mutex_unlock(pool->mutex);
      if (task != NULL) (*(task->task))(task->args);
    }
  }
  while (1);

  pthread_exit(NULL);
  return NULL;
}
