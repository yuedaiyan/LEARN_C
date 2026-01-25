/*

*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    FILE *hand;
    hand = fopen("romeo.txt","r");
    int i;
    char line[1000];
    while(fgets(line,1000,hand)!=NULL){

        for (i = 0; i < strlen(line); i++)
        {
            putchar(toupper(line[i]));
        }
    }
}

/*





*/
