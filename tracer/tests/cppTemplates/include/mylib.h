#ifndef MYLIB_H_
#define MYLIB_H_

/**
 * Templated fibonacci.
 */
template<uint64_t n>
uint64_t fact() {
  return n*fact<n-1>();
}

/**
 * Specialized base case.
 */
template<>
uint64_t fact<0>() {
  return 1;
}

/**
 * Not Instrumented
 */
template<uint64_t n>
uint64_t fibo() {
  return fibo<n-1>() + fibo<n-2>();
}

/**
 * Specialized base cases.
 */
template<>
uint64_t fibo<1>() {
  return 1;
}

template<>
uint64_t fibo<0>() {
  return 0;
}


#endif // MYLLIB_H_
