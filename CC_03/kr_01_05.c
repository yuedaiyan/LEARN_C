# include <stdio.h>
int count(char s[]){
    int i=0;
    for (i=0; s[i]; i++){}
    return i;
}

int main(){

    char x[] = "Hello";
    int k = count(x);
    printf("%d\n",k );
}

