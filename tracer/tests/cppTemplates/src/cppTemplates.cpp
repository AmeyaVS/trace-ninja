#include <iostream>
#include "mylib.h"

#define N 20

int main() {
  std::cout << "Value of fibo(" << N << "): " << fibo<N>() << std::endl;
  std::cout << "Value of fact(" << N << "): " << fact<N>() << std::endl;
  //std::cout << "Sizeof: " << sizeof(unsigned long) << std::endl;
  return 0;
}
