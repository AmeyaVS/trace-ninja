#ifndef TRACER_H_
#define TRACER_H_

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct traceAttributes {
  char type; //< Entry or Exit
  void *func; //< Current function address
  void *callsite; //< Parent function
  pthread_t currentThread; //< Current thread handle
  struct timespec tInstant; //< Time Instant
};


#endif // TRACER_H_
