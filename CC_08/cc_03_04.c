#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- 数据结构定义 (来自 Page 4) --- */
struct dnode {
    char* key;
    char* value;
    struct dnode* next;
};

struct pydict {
    struct dnode* head;
    struct dnode* tail;
    int count;
};

/* --- 构造函数 (来自 Page 4) --- */
struct pydict* pydict_new() {
    struct pydict* p = malloc(sizeof(*p));
    p->head = NULL;
    p->tail = NULL;
    p->count = 0;
    return p;
}

/* --- 析构函数 (来自 Page 4) --- */
void pydict_del(struct pydict* self) {
    struct dnode *cur, *next;
    cur = self->head;
    while (cur) {
        free(cur->key);
        free(cur->value);
        next = cur->next;
        free(cur);
        cur = next;
    }
    free((void*)self);
}

/* --- 查找方法 (对应 Page 6 "Six lines of code") --- */
struct dnode* pydict_find(struct pydict* self, char* key) {
    struct dnode* cur = self->head;
    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

/* --- 获取方法 (对应 Page 6 "Two lines of code") --- */
char* pydict_get(struct pydict* self, char* key) {
    struct dnode* entry = pydict_find(self, key);
    if (entry)
        return entry->value;
    return NULL;
}

/* --- 获取长度 (对应 Page 5 "One line of code") --- */
int pydict_len(const struct pydict* self) {
    return self->count;
}

/* --- 打印方法 (对应 Page 5 "Some code") --- */
/* 目标格式: {'z': 'W', 'y': 'B', 'c': 'C', 'a': 'D'} */
void pydict_print(struct pydict* self) {
    printf("{");
    struct dnode* cur = self->head;
    while (cur) {
        printf("'%s': '%s'", cur->key, cur->value);
        if (cur->next != NULL) {
            printf(", ");
        }
        cur = cur->next;
    }
    printf("}\n");
}

/* --- 插入/更新方法 (对应 Page 6, 9-12) --- */
/*
   注意：由于 pydict_del 会 free(key) 和 free(value)，
   我们在插入时必须为字符串分配内存 (使用 strdup)。
   如果是 POSIX 系统通常自带 strdup，如果是严格 ANSI C 环境可能需要自己实现，
   这里假设使用标准库包含的或常见的 strdup。
*/
struct pydict* pydict_put(struct pydict* self, char* key, char* value) {
    struct dnode* old = pydict_find(self, key);

    if (old != NULL) {
        /* Key 存在：更新操作 (Page 9) */
        /* 必须先释放旧的 value，防止内存泄漏 */
        free(old->value);
        /* 分配新内存并赋值 */
        old->value = strdup(value);
    } else {
        /* Key 不存在：追加操作 (Page 10-12) */
        struct dnode* new_node = malloc(sizeof(struct dnode));
        new_node->key = strdup(key);
        new_node->value = strdup(value);
        new_node->next = NULL;

        /* 链表连接逻辑 */
        if (self->head == NULL) {
            self->head = new_node;
        } else {
            self->tail->next = new_node;
        }
        self->tail = new_node;
        self->count++;
    }
    return self;
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
    for (struct dnode* cur = dct->head; cur != NULL; cur = cur->next) {
        printf("%s=%s\n", cur->key, cur->value);
    }

    pydict_del(dct);

    return 0;
}