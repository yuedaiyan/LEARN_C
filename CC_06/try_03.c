#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char user_input[100];
  char command[200];

  printf("我是复读机，请输入一段话，我会用系统命令把它打印出来：\n");
  printf("> ");

  // 1. 获取用户输入
  // 使用 fgets 读取一行输入 (注意：fgets 本身能防止缓冲区溢出，但防不了注入)
  fgets(user_input, sizeof(user_input), stdin);

  // 去掉输入末尾的换行符 (纯粹为了处理字符串格式)
  user_input[strcspn(user_input, "\n")] = 0;

  // 2. 【漏洞所在】构建命令字符串
  // 程序原本的意图只是执行 echo 命令打印一句话
  // 类似于漫画里的 "INSERT INTO students..."
  sprintf(command, "echo 你输入的内容是: %s", user_input);

  // 3. 执行系统命令
  // system() 会调用操作系统的命令行（Shell/CMD）来执行字符串
  printf("\n[调试信息] 系统即将执行的完整命令是: [%s]\n", command);
  printf("----------------执行结果----------------\n");
  system(command);

  return 0;
}