#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 1. 必须包含此头文件以使用 strlen 和 strcpy

#define MAXLINE 1024 // 2. 定义缓冲区的大小

typedef struct Task {
    char *text;
    struct Task *prev;
    struct Task *next;
} Task;

int main() {
    Task *head = NULL;
    Task *tail = NULL;

    char line[MAXLINE];
    printf("\nplease input:");
    while (fgets(line, MAXLINE, stdin) != NULL) {

        char *save = (char *)malloc(strlen(line) + 1);
        strcpy(save, line);
        Task *new_task = (Task *)malloc(sizeof(Task));
        new_task->text = save;

        if (head == NULL) {
            head = new_task;
            new_task->next = NULL;
            new_task->prev = NULL;
        }
        if (tail != NULL) {
            tail->next = new_task;
            new_task->prev = tail;
        }
        tail = new_task;

        printf("\nplease input: ");
    }
    printf("\ninput (while) is over\n");

    Task *curr = tail;
    while (curr != NULL) {
        printf("%s", curr->text);
        curr = curr->prev;
    }
}