#include <stdio.h>

// 1. 修改：第一个参数直接接收 char 值，不再传指针
// 2. 新增：增加 total 参数，方便计算总数
void analyze_char(char c, int *n_lines, int *n_tabs, int *n_spaces,
                  int *n_total) {
  (*n_total)++; // 每读一个字符，总数+1

  switch (c) {
  case '\n':
    (*n_lines)++;
    break;
  case '\t':
    (*n_tabs)++;
    break;
  case ' ':
    (*n_spaces)++;
    break;
  default:
    // 其他字符不做处理，但已经计入 n_total
    break;
  }
}

int main() {
  int c; // getchar 返回的是 int
  int n_lines = 0, n_tabs = 0, n_spaces = 0, total = 0;

  printf("请输入文本 (按 Ctrl+D 或 Ctrl+Z 结束):\n");

  // 3. 修改：使用 getchar() 读取输入流，直到 EOF
  while ((c = getchar()) != EOF) {
    // 直接传值 c
    analyze_char(c, &n_lines, &n_tabs, &n_spaces, &total);
  }

  // 计算其他字符
  int other = total - n_lines - n_tabs - n_spaces;

  printf("\n=== 分析报告 ===\n");
  printf("换行符 (Lines) : %d\n", n_lines);
  printf("制表符 (Tabs)  : %d\n", n_tabs);
  printf("空格 (Spaces)  : %d\n", n_spaces);
  printf("其他字符       : %d\n", other);
  printf("----------------\n");
  printf("总字符数       : %d\n", total);

  return 0;
}