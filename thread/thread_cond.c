#include <pthread.h>
#include "adk/base.h"
#include "adk/debug.h"
#include "adk/thread.h"

static pthread_cond_t *cond;
static pthread_mutex_t *mutex;

static uint8_t token = 0;

void *swith_push(void *args)
{
  if (args)
  {
    /* Ignore Arguments. */
  }
  static uint32_t count = 0;
  pthread_t tid = pthread_self();
  for (;;)
  {
    if (!pthread_mutex_lock(mutex))
    {
      if (count == 9)
      {
        pthread_mutex_unlock(mutex);
        break;
      }
      pthread_cond_wait(cond, mutex);
      if (token)
      {
        log_info("(Thread ID %u) push %d times.", (unsigned int)tid, count++);
        token--;
      }
      pthread_mutex_unlock(mutex);
      sleep(1);
    }
    else
    {
      log_err("Unable to lock the mutex.");
      break;
    }
  }

  pthread_exit(NULL);
  return NULL;
}

void *switch_pop(void *args)
{
  if (args)
  {
    /* Ignore Arguments. */
  }
  static uint32_t count = 0;
  pthread_t tid = pthread_self();
  for (;;)
  {
    if (!pthread_mutex_lock(mutex))
    {
      if (count == 10)
      {
        pthread_mutex_unlock(mutex);
        break;
      }
      if (!token)
      {
        log_info("(Thread ID %u) pop %d times.", (unsigned int)tid, count++);
        token++;
      }
      pthread_cond_signal(cond);
      pthread_mutex_unlock(mutex);
      sleep(1);
    }
    else
    {
      log_err("Unable to lock the mutex.");
      break;
    }
  }

  pthread_exit(NULL);
  return NULL;
}

int main(void)
{
  cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
  mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
  pthread_cond_init(cond, NULL);
  pthread_mutex_init(mutex, NULL);

  pthread_t thread_0;
  pthread_t thread_1;

  int res_0 = pthread_create(&thread_0, NULL, swith_push, NULL);
  int res_1 = pthread_create(&thread_1, NULL, switch_pop, NULL);

  if (res_0 || res_1)
  {
    log_err("Can not create other thread.");
    exit(-1);
  }

  pthread_join(thread_0, NULL);
  pthread_join(thread_1, NULL);

  pthread_mutex_lock(mutex);
  pthread_mutex_destroy(mutex);
  if (mutex != NULL)
  {
    log_info("Mutex ptr exits. Destroy it.");
    free(mutex);
  }

  pthread_cond_destroy(cond);
  if (cond != NULL)
  {
    log_info("Cond ptr exits. Destroy it.");
    free(cond);
  }

  return 0;
}
