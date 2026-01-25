#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==========================================
 * 第一部分：Python 1.0 风格列表 (动态数组)
 * 对应 PPT Slide 5, 6, 7, 8, 10
 * ========================================== */

// 定义列表结构体 (Slide 5)
struct p1list {
    int alloc; // 分配的内存容量
    int length; // 当前实际元素个数
    char** items; // 指向字符串数组的指针 (指针的指针)
};

// 构造函数：初始化列表 (Slide 5)
struct p1list* p1list_new() {
    struct p1list* p = malloc(sizeof(*p));
    p->length = 0;
    p->alloc = 2; // 初始容量设为2，方便演示扩容
    p->items = malloc(p->alloc * sizeof(char*));
    return p;
}



// 添加元素函数 (Slide 6, 7, 8, 10)
void p1list_append(struct p1list* self, char* str) {
    // 如果满了，就扩容 (Extend if necessary)
    if (self->length >= self->alloc) {
        printf("Extending from %d to %d\n", self->alloc, self->alloc + 2);
        self->alloc = self->alloc + 2;
        // 核心：realloc 重新分配更大的内存块，并保留原有数据
        self->items = (char**)realloc(self->items, (self->alloc * sizeof(char*)));
    }

    // 分配内存保存字符串副本 (Slide 6)
    char* saved = malloc(strlen(str) + 1);
    strcpy(saved, str);

    // 将指针存入数组
    self->items[self->length] = saved;
    self->length++;
}

// (辅助函数) 打印 p1list 的内容，方便观察结果
void p1list_print(struct p1list* self) {
    printf("--- Python 1.0 List (Array) ---\n");
    printf("Alloc: %d, Length: %d\n", self->alloc, self->length);
    for (int i = 0; i < self->length; i++) {
        printf("[%d] %s\n", i, self->items[i]);
    }
    printf("\n");
}

/* ==========================================
 * 第二部分：K&R 风格列表 (链表)
 * 对应 PPT Slide 9, 10
 * ========================================== */

// 定义节点结构体 (PPT中未直接显示定义，但在Slide 10使用了)
struct lnode {
    char* text; // 数据
    struct lnode* next; // 指向下一个节点的指针
};

// 定义链表结构体
struct krlist {
    struct lnode* head; // 头指针
    struct lnode* tail; // 尾指针 (为了快速追加)
    int count; // 计数器
};

// 构造函数 (PPT未显示，为了代码完整性补充)
struct krlist* krlist_new() {
    struct krlist* k = malloc(sizeof(*k));
    k->head = NULL;
    k->tail = NULL;
    k->count = 0;
    return k;
}

// 添加元素函数 (Slide 10 右侧代码)
void krlist_append(struct krlist* self, char* str) {
    struct lnode* new = malloc(sizeof(*new));
    new->next = NULL;

    // 如果是第一个节点，头也是它
    if (self->head == NULL)
        self->head = new;
    // 如果之前有尾巴，旧尾巴的next指向新节点
    if (self->tail != NULL)
        self->tail->next = new;
    // 更新尾巴为新节点
    self->tail = new;

    // 分配内存保存字符串副本
    char* text = malloc(strlen(str) + 1);
    strcpy(text, str);
    new->text = text;

    self->count++;
}

// (辅助函数) 打印 krlist 的内容
void krlist_print(struct krlist* self) {
    printf("--- K&R List (Linked List) ---\n");
    printf("Count: %d\n", self->count);
    struct lnode* current = self->head;
    int i = 0;
    while (current != NULL) {
        printf("Node->text: %s\n", current->text);
        current = current->next;
        i++;
    }
    printf("\n");
}

/* ==========================================
 * 主函数：模拟 PPT 的演示流程
 * ========================================== */
int main() {
    // 1. 演示 Python 1.0 风格列表 (动态数组)
    printf("=== Demo 1: Python 1.0 List ===\n");
    struct p1list* plist = p1list_new();

    p1list_append(plist, "C");
    p1list_append(plist, "is");
    // 下一步会触发扩容 (因为初始 alloc=2, 存入第3个时需要扩容)
    p1list_append(plist, "Fun");

    p1list_print(plist);

    // 2. 演示 K&R 风格列表 (链表)
    printf("=== Demo 2: K&R Linked List ===\n");
    struct krlist* klist = krlist_new();

    krlist_append(klist, "C");
    krlist_append(klist, "is");
    krlist_append(klist, "Fun");

    krlist_print(klist);

    return 0;
}