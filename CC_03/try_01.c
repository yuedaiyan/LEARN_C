# include <stdio.h>
# include<string.h>
int main() {
    char x[100];
    printf("input word:\n");
    fgets(x, 100, stdin);
    int length = strlen(x);
    int i = length - 2;
    for (; i>=0; i--)
    {
        printf("%c\n", x[i]);
    }
}
