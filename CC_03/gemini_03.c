#include <stdio.h>

void swap(int *a,int *b){
    int temp=*a;
    *a = *b;
    *b = temp;
}

int main(){

    int a, b;
    swap(&a, &b);
}