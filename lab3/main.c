#include <stdio.h>
#include <stdlib.h>

const int a[] = {1, 2, 3};
const int b[] = {4, 5, 6};

int scalar(const int a[], const int b[], size_t size) {
  int result = 0;
  for (int i = 0; i < size; i++) {
    result += a[i] * b[i];
  }
  return result;
}

int is_prime(unsigned long n) {
  if (1 == n || 0 == n) return 0;
  for (int k = 2; k*k <= n; k++) {
    if (n % k == 0) return 0;
  }
  return 1;
}

int main() {
  printf("Scalar product: %d\n", scalar(a, b, sizeof(a) / sizeof(a[0])));

  printf("Enter the number to check for primality: ");
  unsigned long num = 0;
  scanf("%lu", &num);
  printf("Primality test result: %d\n", is_prime(num));

  return 0;
}

