#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 1. 必须包含此头文件以使用 strlen 和 strcpy

#define MAXLINE 1024 // 2. 定义缓冲区的大小

typedef struct Task {
  char *content_ptr;
  struct Task *next_ptr;
} Task;

Task *head_ptr = NULL;
Task *tail_ptr = NULL;

int main() {
  char line[MAXLINE];
  while (fgets(line, MAXLINE, stdin) != NULL) {
    // char *save_ptr[sizeof(line) + 1];
    char *save_ptr = (char *)malloc(strlen(line) + 1);
    if (save_ptr == NULL) {
      return 1;
    }
    strcpy(save_ptr, line);
    //
    Task *new_struct = (Task *)malloc(sizeof(Task));
    if (new_struct == NULL) {
      return 1;
    }
    new_struct->next_ptr = NULL;
    if (head_ptr == NULL) {
      head_ptr = new_struct;
    }
    //

    if (tail_ptr != NULL) {
      tail_ptr->next_ptr = new_struct;
    }
    tail_ptr = new_struct;
    //

  } // while } end

  // read
  Task *curr_ptr = head_ptr;
  while (curr_ptr != NULL) {
    printf("%s", curr_ptr->content_ptr);
    curr_ptr = curr_ptr->next_ptr;
  }

  // free
  curr_ptr = head_ptr;
  while (curr_ptr != NULL) {
    Task *temp_ptr = curr_ptr;
    curr_ptr = curr_ptr->next_ptr;
    free(temp_ptr->content_ptr);
    free(temp_ptr);
  }
  return 0;
}

/*
adsfafda

A[iadfasdfahttps://www.youtube.com/watch?v=NnO_PAnX_uA]
*/