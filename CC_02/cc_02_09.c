/*

*/
#include <stdio.h>
int main() {
    int guess;
    while(scanf("%d",&guess)!=EOF){
        if (guess==42){
            printf("correct\n");
            break;
        }
        else if (guess<42)
            printf("too small\n");
        
        else
            printf(
                "too big\n");
    }
}

/*





*/
