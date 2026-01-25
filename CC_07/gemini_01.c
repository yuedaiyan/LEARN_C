#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 1. 必须包含此头文件以使用 strlen 和 strcpy

#define MAXLINE 1024 // 2. 定义缓冲区的大小

/*--------------------------------*/

typedef struct Task {
    char *text_ptr;
    struct Task *next_ptr;
} Task;
struct Task *head_ptr = NULL;
struct Task *tail_ptr = NULL;

/*--------------------------------*/
int main() {
    char line_ptr[MAXLINE];
    while (fgets(line_ptr, MAXLINE, stdin) != NULL) {
        char *save_ptr = (char *)malloc(strlen(line_ptr) + 1);
        if (save_ptr == NULL) {
            return 1;
        }

        strcpy(save_ptr, line_ptr);
        Task *new_Task_ptr = (Task *)malloc(sizeof(Task));
        if (new_Task_ptr == NULL) {
            return 1;
        }
        new_Task_ptr->text_ptr = save_ptr;

        new_Task_ptr->next_ptr = NULL;

        if (head_ptr == NULL) {
            head_ptr = new_Task_ptr;
        }
        if (tail_ptr != NULL) {
            tail_ptr->next_ptr = new_Task_ptr;
        }

        tail_ptr = new_Task_ptr;
    }

    // 输出
    Task *curr_ptr = head_ptr;
    while (curr_ptr != NULL) {
        printf("%s", curr_ptr->text_ptr);
        curr_ptr = curr_ptr->next_ptr;
    }
    // 清除
    Task *clean_ptr = head_ptr;
    while (clean_ptr != NULL) {
        Task *temp_ptr = clean_ptr;
        clean_ptr = clean_ptr->next_ptr;
        free(temp_ptr->text_ptr);
        free(temp_ptr);
    }
    head_ptr = NULL;
    tail_ptr = NULL;
}