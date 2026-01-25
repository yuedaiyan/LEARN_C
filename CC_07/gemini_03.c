#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syslimits.h>

#define MAXLINE 1024

typedef struct Task {
    char *text_ptr;
    struct Task *next_ptr;
} Task;


Task *head_ptr = NULL;
Task *tail_ptr = NULL;
/*-----------------------*/
void free_node(int delet_node_int) {
    Task *prevNode_ptr = NULL;
    Task *currNode_ptr = head_ptr;
    int count = 0;

    if (delet_node_int == 0) {
        if (currNode_ptr->next_ptr == NULL) {
            tail_ptr = NULL;
        }
        head_ptr = head_ptr->next_ptr;
        free(currNode_ptr->text_ptr);
        free(currNode_ptr);
        return;
    }


    // Main while
    while (count < delet_node_int && currNode_ptr != NULL) {
        count++;
        prevNode_ptr = currNode_ptr;
        currNode_ptr = prevNode_ptr->next_ptr;
    }


    // Input index too big
    if (currNode_ptr == NULL) {
        printf("\nInput index too big!\n");
        return;
    }


    // If delete is tail node
    if (currNode_ptr->next_ptr == NULL) {
        tail_ptr = prevNode_ptr;
    }


    // Joint prevNode's next_ptr to nextNode's next_ptr
    prevNode_ptr->next_ptr = currNode_ptr->next_ptr;

    // Free memory
    free(currNode_ptr->text_ptr);
    free(currNode_ptr);

    printf("\nDelet index node success\n");
}

/*-----------------------*/
int main() {
    printf("main function start\n");
    printf("please input:\n");
    char line[MAXLINE];
    while (fgets(line, MAXLINE, stdin) != NULL) {

        printf("please input:\n");
        char *temp_save_ptr = (char *)malloc(strlen(line) + 1);
        if (temp_save_ptr == NULL) {
            return 1;
        }
        strcpy(temp_save_ptr, line);
        Task *new_task = (Task *)malloc(sizeof(Task));

        new_task->text_ptr = temp_save_ptr;
        new_task->next_ptr = NULL;

        if (head_ptr == NULL) {
            head_ptr = new_task;
        }
        if (tail_ptr != NULL) {
            tail_ptr->next_ptr = new_task;
        }

        tail_ptr = new_task;
    }
    printf("input(while)has end.\n");

    // 维持可持续输入状态
    clearerr(stdin);

    // Delete one node
    printf("\nStart delete one node.\nPlease input delete index:");
    char buf[16];
    fgets(buf, sizeof(buf), stdin);
    // scanf("%d", &deleteIndex);
    int deleteIndex_int = atoi(buf);
    free_node(deleteIndex_int);


    printf("\nStart read Task.\n");
    Task *curr_ptr = head_ptr;


    // Read out Task
    while (curr_ptr != NULL) {
        printf("Task: %s", curr_ptr->text_ptr);
        curr_ptr = curr_ptr->next_ptr;
    }
    printf("Read Task end.\n");


    printf("\nStart free memory.\n");
    // Free memory
    curr_ptr = head_ptr;
    while (curr_ptr != NULL) {
        Task *temp_ptr = NULL;
        temp_ptr = curr_ptr;
        curr_ptr = curr_ptr->next_ptr;
        free(temp_ptr->text_ptr);
        free(temp_ptr);
    }
    printf("Free memory end.\n");
}