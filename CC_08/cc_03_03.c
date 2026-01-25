#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 定义节点结构体 (Page 3) */
struct lnode {
    char* text;
    struct lnode* next;
};

/* 定义列表结构体 (Page 3) */
struct pylist {
    struct lnode* head;
    struct lnode* tail;
    int count;
};

/* 构造函数 - pylist_new (Page 3) */
struct pylist* pylist_new() {
    struct pylist* p = malloc(sizeof(*p));
    p->head = NULL;
    p->tail = NULL;
    p->count = 0;
    return p;
}

/* 析构函数 - pylist_del (Page 3 & 4) */
/* 重要：必须先释放节点内的字符串，再释放节点本身 */
void pylist_del(struct pylist* self) {
    struct lnode *cur, *next;
    cur = self->head;
    while (cur) {
        free(cur->text); // 释放字符串内存
        next = cur->next; // 先保存下一个节点的指针
        free(cur); // 释放当前节点结构体
        cur = next; // 移动到下一个
    }
    free((void*)self); // 最后释放列表结构体本身
}

/* 打印列表 - pylist_print (Page 5) */
/* 目标输出格式: ['Hello world', 'Catch phrase'] */
void pylist_print(struct pylist* self) {
    printf("[");
    struct lnode* cur = self->head;
    while (cur) {
        printf("'%s'", cur->text); // 打印带单引号的字符串
        if (cur->next != NULL) {
            printf(", "); // 如果不是最后一个元素，打印逗号和空格
        }
        cur = cur->next;
    }
    printf("]\n"); // 结尾打印右括号和换行
}

/* 获取长度 - pylist_len (Page 6) */
/* 要求：一行代码 */
int pylist_len(const struct pylist* self) {
    return self->count;
}

/* 添加元素 - pylist_append (Page 6) */
void pylist_append(struct pylist* self, char* str) {
    // 1. 创建新节点
    struct lnode* node = malloc(sizeof(struct lnode));

    // 2. 复制字符串 (strdup 分配内存并复制内容，这很重要，因为析构函数会free它)
    // 注意：如果编译器不支持 strdup (非标准C)，可以使用 malloc + strcpy 替代
    node->text = strdup(str);
    node->next = NULL;

    // 3. 将节点链接到链表中
    if (self->head == NULL) {
        // 如果是空列表，head 和 tail 都指向新节点
        self->head = node;
        self->tail = node;
    } else {
        // 如果非空，添加到 tail 之后，并更新 tail
        self->tail->next = node;
        self->tail = node;
    }

    // 4. 增加计数
    self->count++;
}

/* 查找索引 - pylist_index (Page 6) */
/* 如果找到返回索引，否则返回 -1 */
int pylist_index(struct pylist* self, char* str) {
    struct lnode* cur = self->head;
    int index = 0;
    while (cur) {
        // 使用 strcmp 比较字符串内容，相等返回 0
        if (strcmp(cur->text, str) == 0) {
            return index;
        }
        cur = cur->next;
        index++;
    }
    return -1;
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