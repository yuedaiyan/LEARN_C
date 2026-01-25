#include <stdio.h>
#include <string.h> // 为了使用 strlen

void reverseString(char *line_char, int length_int){
    printf("\n---start reverseString---\n");
    printf("line_char %%c: %c\n", *line_char);
    printf("length_int %%d: %d\n", length_int);
    printf("old line: %s\n", line_char);
    int l_c = 0;
    int r_c = length_int-2;
    for (;l_c<r_c;l_c++){
        char temp_char;
        printf("l_c=%d, r_c=%d,l_c:%c,r_c:%c\n", l_c, r_c,line_char[l_c],line_char[r_c]);
        temp_char = line_char[l_c];
        line_char[l_c] = line_char[r_c];
        line_char[r_c] = temp_char;
        r_c--;
    }

    printf("---reverseString end---\n\n");

}

int main(){
 
    char line[100];
    printf("please input:\n");
    fgets(line, 100, stdin);
 
 
    //char line[6]="ABCDE";
    
    int length = strlen(line);
    printf("input success\n");
    printf("\n");
    printf("length(include \\n): %d\n",length);
    printf("&length: %p\n", &length);
    printf("*&length: %d\n", *&length);
    printf("\n");
    printf("old line: %s\n", line);
    printf("&line: %p\n", &line);
    //
    reverseString(line, length);
    //
    printf("new line: %s\n", line);

}