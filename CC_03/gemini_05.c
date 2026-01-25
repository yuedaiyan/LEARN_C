#include <stdio.h>
#include <string.h> // 为了使用 strlen

void reverseString(char *line_char){
    //char temp;
    printf("\n---start reverseString---\n");
    printf("line_char %%p:%p\n", line_char);
    printf("line_char %%c:%c\n", *line_char);
    //char left_char = line_char;
    printf("\n");
    char *new_char = line_char + 1;

    printf("new_char %%p:%p\n", new_char);
    printf("new_char %%c:%c\n", *new_char);

    printf("---reverseString end---\n\n");
}

int main() {
 
    //char line[100];
    //printf("please input:\n");
    //fgets(line, 100, stdin);
 
 
    char line[6]="ABCDE";
    
    int length = strlen(line);
    printf("\ninput success\n");
    printf("\n");
    printf("length(include \\n): %d\n",length);
    printf("&length: %p\n", &length);
    printf("*&length: %d\n", *&length);
    printf("\n");
    printf("old line: %s\n", line);
    printf("&line: %p\n", &line);
    //
    reverseString(line);
    //
    printf("new line: %s\n", line);

}