#include <stdio.h>


void reverseString(char *line_ad){
    printf("\n---start reverseString---\n");
    char *left_char_ad = line_ad;
    char * right_char_ad=line_ad;

    for (; *right_char_ad!= '\0'; right_char_ad++);
    right_char_ad=right_char_ad-2;
    
    while (left_char_ad < right_char_ad) {
 
        printf("left_char_ad  %%p:%p -> left_char_ad  %%c:%c\n", left_char_ad,*left_char_ad);
        printf("right_char_ad %%p:%p -> right_char_ad %%c:%c\n", right_char_ad,*right_char_ad);
        
        char temp_c = *left_char_ad;
        *left_char_ad = *right_char_ad;
        *right_char_ad = temp_c;

        left_char_ad++;
        right_char_ad--;

        printf("\n--add and sub--\n");
        printf("left_char_ad  %%p:%p -> left_char_ad  %%c:%c\n", left_char_ad,*left_char_ad);
        printf("right_char_ad %%p:%p -> right_char_ad %%c:%c\n", right_char_ad,*right_char_ad);
    }
    printf("---reverseString end---\n\n");
}

int main(){
    //char line[100];
    //printf("please input:\n");
    //fgets(line, 100, stdin);
    char line[6]="ABCDE\n";

    printf("\ninput success\n");
    printf("\n");

    printf("\n");
    printf("old line: %s\n", line);
    printf("&line: %p\n", &line);
    reverseString(line);
    printf("new line: %s\n", line);

}