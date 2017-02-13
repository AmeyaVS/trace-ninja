#include <iostream>

#define N 20

uint64_t fibo(uint64_t n) {
  uint64_t ret_val = 0;
  switch(n) {
    case 0:
      break;
    case 1:
      ret_val = 1;
      break;
    default:
      ret_val = fibo(n-1) + fibo(n-2);
      break;
  }
  return ret_val;
}

static uint64_t fact(uint64_t n) {
  return n?n*fact(n-1):1;
}

int main() {
  for(uint64_t i = 0; i <= N; ++i) {
    std::cout << "fibo(" << i << "): " << fibo(i) << ", fact(" << i << "): " << fact(i) << std::endl;
  }
  return 0;
}
