#include <stdio.h>

void func(int a, int *pb) {
  a = 1;
  *pb = 2;
}

int main() {
  int x, y;

  x = 42;
  y = 43;
  printf("main x=%d y=%d\n", x, y);
  func(x, &y);
  printf("back x=%d y=%d\n", x, y);
}
