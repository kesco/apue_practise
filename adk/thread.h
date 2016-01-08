#ifndef __ADK_THREAD_H__
#define __ADK_THREAD_H__

#include <pthread.h>

#ifndef __ADK_BASE_H__
#include <stdint.h>
#endif

typedef struct thread_pool_t
{
  pthread_t *thread;
  pthread_cond_t *cond;

  uint32_t capacity;
} thread_pool_t;

enum thread_pool_state
{
  RUNNING = 0,
  HALT = -1
};

#endif /* __ADK_THREAD_H__ */
