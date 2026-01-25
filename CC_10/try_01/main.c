// main.c
#include "my_math.h" // 引入“菜单”，这样编译器就知道有这几个函数了
#include <stdio.h>

int main() {
    int x = 10, y = 5;

    printf("加法: %d\n", add(x, y));
    printf("减法: %d\n", sub(x, y));
    printf("乘法: %d\n", mul(x, y));
    printf("除法: %.2f\n", div(x, y));

    return 0;
}