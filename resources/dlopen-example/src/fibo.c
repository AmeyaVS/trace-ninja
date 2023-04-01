#include "fibo.h"

unsigned int fibo(unsigned int n) {
  unsigned int ret = 0;
  switch(n) {
    case 0:
      ret = 0;
      break;
    case 1:
      ret = 1;
      break;
    default:
      ret = fibo(n - 2) + fibo(n - 1);
      break;
  }
  return ret;
}
