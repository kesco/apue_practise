#include "adk/base.h"
#include "adk/debug.h"
#include "adk/thread.h"

void* thread_create_handler(void* args)
{
  if (!args)
  {
    log_info("Thread Recevice nothing.");
  } else {
    char* msg = (char*) args;
    log_info("Thread Recevice: %s",msg);
  }
  return ((void*) 0);
}

int main()
{
  pthread_t ptt;
  int res;
  res = pthread_create(&ptt, NULL, thread_create_handler, NULL);
  if(res == EAGAIN) {
    log_err("Can not create other thread.");
    exit(-1);
  }
  log_info("Res #0 %d", res);
  res = pthread_join(ptt, NULL);
  log_info("Res #1 %d", res);
  return 0;
}
