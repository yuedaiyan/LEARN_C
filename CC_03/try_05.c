#include <stdio.h>
#include <string.h> // 为了使用 strlen
int main() {
  long a = 0x16bac6d08;
  long b = 0x16f506d4d;
  long c = b-a;
  printf("c:%ld\n",c);
    return 0;
}