#include "adk/debug.h"
#include "adk/thread.h"
#include <unistd.h>

void run(void *arg)
{
  static int count = 0;
  int *token = (int *)arg;
  count += 1;
  log_info("%d:%d", count, *token);
  sleep(3);
}

int main()
{
  thread_pool_t *pool = thread_pool_create(200, 50);
  do
  {
    if (pool == NULL)
    {
      log_err("Can not create a thread pool");
      break;
    }
    for (int i = 0; i < 50; i += 1)
    {
      thread_pool_add_task(pool, run, &i);
    }
    sleep(5);
    thread_pool_destroy(pool);
  }
  while (0);
  log_info("Ending"); 
  return 0;
}
