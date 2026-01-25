#include <stdio.h>

int main() {
  char src[] = "Hello World";

  // 1. src 只是指向第一个字符
  // src + 1 会跳过 1 个字节 (sizeof(char))
  printf("src + 1 指向: %c\n", *(src + 1)); // 输出 'e'

  // 2. &src 是指向整个数组
  // &src + 1 会跳过 12 个字节 (整个数组的大小)
  printf("地址差值: %ld\n", (char *)(&src + 1) - (char *)(&src)); // 输出 12

  return 0;
}