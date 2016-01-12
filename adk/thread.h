#ifndef __ADK_THREAD_H__
#define __ADK_THREAD_H__

typedef struct thread_pool_t thread_pool_t;

typedef enum
{
  RUNNING = 0,
  HALT = -1
} thread_pool_state;

thread_pool_t *thread_pool_create(int thread_size, int queue_count);
int thread_pool_add_task(thread_pool_t *pool, void (*task)(void *), void *args);
int thread_pool_destroy(thread_pool_t *pool);

#endif /* __ADK_THREAD_H__ */
