#include <_string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 1000
// 1.列表节点
typedef struct list_node {
    char* text;
    struct list_node* next;
} list_node;

typedef struct list_list {
    list_node* head;
    list_node* tail;
    int count;
} list_list;

// 2.创建列表
struct list_list* creat_list() {
    list_list* new_list = (list_list*)malloc(sizeof(list_list));
    if (new_list == NULL) {
        printf("NO memory");
        return NULL;
    }
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->count = 0;
    return new_list;
}

// 3.追加元素
void list_add(list_list* self_list) {
    char line[MAXCHAR];
    if (fgets(line, MAXCHAR, stdin) != NULL) {
        line[strcspn(line, "\n")] = 0;
        //
        list_node* new_node = (list_node*)malloc(sizeof(list_node));
        if (new_node == NULL)
            return;

        new_node->text = strdup(line);
        new_node->next = NULL;

        self_list->count++;
        if (self_list->tail != NULL) {
            self_list->tail->next = new_node;
        }
        self_list->tail = new_node;
        if (self_list->head == NULL) {
            self_list->head = new_node;
        }
        return;
    } else {
        return;
    }
}

// 4.功能：获取列表长度
int length_list(list_list* self_list) {
    return self_list->count;
}

// 5.打印列表
void print_list(list_list* self_list) {
    list_node* curr = self_list->head;
    while (curr != NULL) {
        printf("\n%s\n", curr->text);
        curr = curr->next;
    }
    return;
}

// 6.查找元素索引
int index_list(list_list* self_list, char* str_index) {
    int list_index = 0;
    list_node* curr = self_list->head;
    while (curr != NULL) {
        if (strcmp(str_index, curr->text) == 0) {
            return list_index;
        } else {
            list_index++;
        }
        curr = curr->next;
    }
    printf("can't find the str\n");
    return -1;
}

// 7.清除列表
void delete_list(list_list* self_list) {
    list_node* curr = self_list->head;
    while (curr != NULL) {
        list_node* prev = curr;
        curr = curr->next;
        free(prev->text);
        free(prev);
    }
    free(self_list);
}

// ==========================================
// 8. 主函数测试
// ==========================================
int main() {
    list_list* my_list = creat_list();

    printf("Enter 3 items:\n");
    for (int i = 0; i < 3; i++) {
        list_add(my_list);
    }

    print_list(my_list);

    // 测试查找
    char search_term[MAXCHAR];
    printf("\nEnter item to search index: ");
    if (fgets(search_term, MAXCHAR, stdin) != NULL) {
        search_term[strcspn(search_term, "\n")] = 0; // 去掉换行符
        int idx = index_list(my_list, search_term);
        printf("Index: %d\n", idx);
    }

    delete_list(my_list);
    return 0;
}