/*
题目 3：寻找最小值（处理 EOF）

编写一个程序，不断读入用户输入的整数，直到用户按下 Ctrl+D (产生 EOF) 为止 。最后输出这些数字中的最小值。

核心考点：

不能把 minval 简单初始化为 0（万一用户输入的都是正数呢？）。

参考资料中的做法，使用一个 int first = 1; 标志位来处理第一个进入的数字 。


使用 while (scanf("%d", &val) != EOF) 来控制循环 。
*/
#include <stdio.h>

int main() {
    int a,minval;
    int t=0;

    while(scanf("%d",&a)!=EOF){
        if (t==0){
        minval = a;
        t = 1;
    }

        else if (a< minval){
            minval = a;
        }
    }
    printf("%d\n", minval);
}

/*





*/
