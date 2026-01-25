#include <_stdlib.h>
#include <_string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 定义节点结构体 (Page 3) */
typedef struct list_node {
    char* text;
    struct list_node* next;
} list_node;

typedef struct pylist {
    list_node* head;
    list_node* tail;
    int count;
} pylist;

/* 构造函数 - pylist_new (Page 3) */
struct pylist* pylist_new() {
    pylist* nl = (pylist*)malloc(sizeof(*nl));
    if (nl == NULL) {
        printf("Ram error\n");
        return NULL;
    }
    nl->head = NULL;
    nl->tail = NULL;
    nl->count = 0;
    return nl;
}

/* 析构函数 - pylist_del (Page 3 & 4) */
/* 重要：必须先释放节点内的字符串，再释放节点本身 */
void pylist_del(pylist* self) {
    if (self->head == NULL) {
        // printf("No pylist exit\n");
        free(self);
        return;
    }
    list_node* curr = self->head;
    list_node* prev = NULL;
    while (curr) {
        prev = curr;
        curr = curr->next;
        free(prev->text);
        free(prev);
    }
    free(self);
    return;
}

/* 打印列表 - pylist_print (Page 5) */
/* 目标输出格式: ['Hello world', 'Catch phrase'] */
void pylist_print(pylist* self) {
    printf("[");
    list_node* curr = self->head;
    while (curr) {
        printf("'%s'", curr->text);
        if (curr->next != NULL) {
            printf(", ");
        }
    curr=curr->next;
    }
    printf("]\n");
    return;
}
/* 获取长度 - pylist_len (Page 6) */
int pylist_len(pylist* self) {
    return self->count;
}


/* 添加元素 - pylist_append (Page 6) */
void pylist_append(pylist* self, char* str) {
    list_node* new_node = (list_node*)malloc(sizeof(*new_node));
    if (new_node == NULL) {
        printf("Ram error\n");
        return;
    }
    if (self->head == NULL) {
        self->head = new_node;
    } else {
        self->tail->next = new_node;
    }
    new_node->text = strdup(str);
    new_node->next = NULL;
    self->count++;
    self->tail=new_node;
    return;
}

/* 查找索引 - pylist_index (Page 6) */
/* 如果找到返回索引，否则返回 -1 */
int pylist_index(pylist* self, char* str) {
    int index = 0;
    list_node* curr = self->head;
    while (curr) {
        if (strcmp(curr->text, str)==0) {
            return index;
        }
        index++;
        curr=curr->next;
    }return -1;
}

/* 主函数 (Page 2 & 7) */
int main(void) {
    struct pylist* lst = pylist_new();
    pylist_append(lst, "Hello world");
    pylist_print(lst);
    pylist_append(lst, "Catch phrase");
    pylist_print(lst);
    pylist_append(lst, "Brian");
    pylist_print(lst);

    printf("Length = %d\n", pylist_len(lst));
    printf("Brian? %d\n", pylist_index(lst, "Brian"));
    printf("Bob? %d\n", pylist_index(lst, "Bob"));

    pylist_del(lst);
    return 0;
}