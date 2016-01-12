#include "adk/debug.h"
#include "adk/thread.h"

int main()
{
  thread_pool_t *pool = thread_pool_create(200,50);
  return 0;
}
