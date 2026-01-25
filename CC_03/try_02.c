# include <stdio.h>
# include<string.h>
int main() {
    char x[100];
    printf("input word:\n");
    fgets(x, 100, stdin);
    int length = strlen(x);
    length =length - 1;
    printf("length: %d\n",length);
    char c;
    int i = 0;
    if (length % 2 == 0)
    {
        // even
        length = length - 1;
        int upper_middle = length / 2 + 1;
        int lower_middle = length / 2;
        for (; i <= lower_middle;i++){
            c = x[length];

            x[length]=x[i]
        }
    }
    else{
        //odd (incloud 1)


    }



    /*
    
    for (; i>=0; i--)
    {
        printf("%c\n", x[i]);
    }
    */

}
