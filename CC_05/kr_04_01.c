#include <stdio.h>
#include <string.h>

void one(int op) {
  printf("One  op before %d\n", op);
  op = op - 10;
  printf("One  op after  %d\n", op);
}

int main() {
  int ma = 42;
  printf("Main ma before %d\n", ma);
  one(ma);
  printf("Main ma after  %d\n", ma);
}
