// my_math.c
#include "my_math.h"

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
double div(int a, int b) {
    if (b == 0)
        return 0; // 简单处理除以0的情况
    return (double)a / b;
}