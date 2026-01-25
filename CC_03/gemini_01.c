#include <stdio.h>

void levelUp(int *p) {
    // 这里该怎么写？
    // 记住：*p 代表“顺着地址找到那个变量”
    *p=*p + 10;
}

int main() {
    int myLevel = 1;
    // 这里怎么调用函数并把地址传过去？
    levelUp(&myLevel);
    printf("现在的等级是: %d\n", myLevel);
    return 0;
}