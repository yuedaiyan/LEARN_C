/*

*/
#include <stdio.h>
int main() {
    int n, maxval, minval;
    int first = 1;
    while(scanf("%d",&n)!=EOF){
        if(first||n>maxval)
            maxval = n;
        if(first||n<minval)
            minval = n;
        first = 0;
    }
    printf("maxval:%d\n", maxval);
    printf("minval:%d\n", minval);
}

/*




*/
