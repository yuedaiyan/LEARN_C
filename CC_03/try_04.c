
# include <stdio.h>
# include<string.h>
int main() {
    char x[100]="hello";
    int length = strlen(x);
    printf("%d\n", length);
    length--;
    printf("length: %c\n", x[length] );
    char *temp;
    
    

    printf("0: %c\n", x[0]);
    printf("length: %c\n", x[length]);
}
