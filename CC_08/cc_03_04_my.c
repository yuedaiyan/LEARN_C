#include <_string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- 数据结构定义 (来自 Page 4) --- */
typedef struct dict_node {
    char* key;
    char* value;
    struct dict_node* next;
} dict_node;

typedef struct pydict {
    dict_node* head;
    dict_node* tail;
    int count;
} pydict;

/* --- 构造函数 (来自 Page 4) --- */
struct pydict* pydict_new() {
    pydict* npd = (pydict*)malloc(sizeof(*npd));
    npd->head = NULL;
    npd->tail = NULL;
    npd->count = 0;
    return npd;
}

/* --- 析构函数 (来自 Page 4) --- */
void pydict_del(pydict* self) {
    dict_node* curr = self->head;
    dict_node* prev = NULL;
    while (curr != NULL) {
        prev=curr;
        curr = curr->next;
        free(prev->key);
        free(prev->value);
        free(prev);
    }
    free(self);
}

/* --- 查找方法 (对应 Page 6 "Six lines of code") --- */
dict_node* pydict_find(pydict* self, char* new_key) {
    dict_node* curr = self->head;
    while (curr) {
        if (strcmp(curr->key, new_key) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

/* --- 获取方法 (对应 Page 6 "Two lines of code") --- */
char* pydict_get(pydict* self, char* point_key) {
    dict_node* curr = self->head;
    while (curr) {
        if (strcmp(curr->key, point_key) == 0) {
            return curr->value;
        }
        curr = curr->next;
    }
    return NULL;
}

/* --- 获取长度 (对应 Page 5 "One line of code") --- */
int pydict_len(pydict* self) {
    return self->count;
}
/* --- 打印方法 (对应 Page 5 "Some code") --- */
void pydict_print(pydict* self) {
    dict_node* curr = self->head;
    printf("[");
    while (curr) {
        printf("'%s':'%s'", curr->key, curr->value);
        if (curr->next != NULL) {
            printf(",");
        }
        curr = curr->next;
    }
    printf("]\n");
}
/* --- 插入/更新方法 (对应 Page 6, 9-12) --- */
void pydict_put(pydict* self, char* new_key, char* new_value) {
    dict_node* find_node = pydict_find(self, new_key);
    if (find_node) {
        // key exit
        free(find_node->value);
        find_node->value = strdup(new_value);
        return;
    } else {
        dict_node* curr = (dict_node*)malloc(sizeof(*curr));
        //  key don't exit
        if (self->head == NULL) {
            // it is the first
            self->head = curr;
            self->tail = curr;
        } else {
            // not the first
            self->tail->next=curr;
            self->tail = curr;
        }
        curr->key = strdup(new_key);
        curr->value = strdup(new_value);
        curr->next = NULL;
        self->count++;
        return;
    }
}

/* --- 主函数 (完全照抄 Page 3) --- */
int main(void) {
    struct pydict* dct = pydict_new();
    pydict_put(dct, "z", "Catch phrase");
    pydict_print(dct);

    pydict_put(dct, "z", "W");
    pydict_print(dct);

    pydict_put(dct, "y", "B");
    pydict_put(dct, "c", "C");
    pydict_put(dct, "a", "D");
    pydict_print(dct);

    printf("Length =%d\n", pydict_len(dct));

    /* 注意：如果在部分编译器上 %s 打印 NULL 会崩溃，但通常 glibc 会打印 (null)，这符合 PPT 截图 */
    printf("z=%s\n", pydict_get(dct, "z"));
    printf("x=%s\n", pydict_get(dct, "x"));

    printf("\nDump\n");
    for (struct dict_node* cur = dct->head; cur != NULL; cur = cur->next) {
        printf("%s=%s\n", cur->key, cur->value);
    }

    pydict_del(dct);

    return 0;
}