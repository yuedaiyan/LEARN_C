/*
题目 2：安全的“回声”程序

考察重点： 字符串数组声明 、带空格的行输入 fgets 、字符串长度与遍历 。



任务描述： 编写一个程序，要求用户输入一整行话（包含空格），然后程序统计并输出这行话中有多少个字符（不包括最后的换行符）。

输入示例： Hello C Programming

输出示例： You entered 20 characters.

你需要注意的坑：

为什么资料中建议使用 fgets 而不是 scanf("%s", ...) 来读取这一行？ 


字符数组 char line[1000] 的末尾有一个看不见的结束符 \0 。在遍历统计长度时，如何判断字符串结束了？
*/
#include <stdio.h>
#include <string.h>
int main() {
    char line[1000];
    int a;
    a = strlen(fgets(line,1000, stdin));
    printf("%d\n", a-1);
}

/*





*/
