#include <stdio.h>
#include <string.h>

// 定义一个结构体，模拟内存中紧挨着的两个区域
struct MemoryLayout {
  char buffer_A[8]; // 图中的 A (8个格子)
  char variable_B;  // 图中的 B (紧挨着 A)
};

int main() {
  struct MemoryLayout mem;

  // --- 1. 初始状态 ---
  // 把 A 清空
  memset(mem.buffer_A, 0, 8);
  // 把 B 设为 3 (对应图中右上角的初始状态)
  mem.variable_B = 3;

  printf("=== 攻击前 ===\n");
  printf("Buffer A 的内容: '%s'\n", mem.buffer_A);
  printf("变量 B 的数值  : %d\n", mem.variable_B);
  printf("变量 B 的地址  : %p\n", (void *)&mem.variable_B);
  printf("Buffer A[8]地址: %p (理论上这是 B 的位置)\n",
         (void *)&mem.buffer_A[8]);

  // --- 2. 执行溢出攻击 ---
  // 我们要写入 "excessive"
  // 它的长度是 9，加上结尾的 '\0'，总共占 10 个字节
  // 而 buffer_A 只有 8 个字节
  char *input = "excessive";

  // strcpy 是不安全的，它不检查长度，只管复制
  strcpy(mem.buffer_A, input);

  printf("\n...正在写入 'excessive'...\n");

  // --- 3. 溢出后果 ---
  printf("\n=== 攻击后 (溢出发生) ===\n");
  printf("Buffer A 的内容: '%s'\n", mem.buffer_A);

  // 让我们看看 B 变成了什么
  printf("变量 B 的数值  : %d\n", mem.variable_B);
  printf("变量 B 的字符  : '%c'\n", mem.variable_B);

  return 0;
}