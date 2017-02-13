// From:
// https://www.guyrutenberg.com/2013/01/27/using-stdchronohigh_resolution_clock-example/
#include <iostream>
#include <chrono>
//using namespace std;

#if 1 // My changes

#define MAX 20
template<uint64_t n>
uint64_t fibo() {
  return fibo<n-1>() + fibo<n-2>();
}

template<>
uint64_t fibo<1>() {
  return 1;
}

template<>
uint64_t fibo<0>() {
  return 0;
}
#endif

int main()
{
  std::cout << std::chrono::high_resolution_clock::period::den << std::endl;
  auto start_time = std::chrono::high_resolution_clock::now();
  int temp;
  for (int i = 0; i< 242000000; i++)
    temp+=temp;
  auto end_time = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << ":";
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << ":";
  std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << ":";
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() << ":";

#if 1 // My changes
  start_time = std::chrono::high_resolution_clock::now();
  std::cout << "\nfibo(" << MAX << "): " << fibo<MAX>() << std::endl;
  end_time = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << ":";
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << ":";
  std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << ":";
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() << ":\n";

  //std::cout << "\nSize of start_time: " << sizeof(start_time) << std::endl;
  //std::cout << "\nSize of end_time: " << sizeof(start_time) << std::endl;
#endif
  return 0;
}
