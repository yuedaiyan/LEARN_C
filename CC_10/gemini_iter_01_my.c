#include <stdio.h>
#include <stdlib.h>

// 1. 节点结构体
typedef struct Node {
    int value;
    struct Node* next;
} Node;

// 2. 链表结构体
typedef struct List {
    Node* head;
    Node* tail;
} List;

// 3. 迭代器结构体
typedef struct Iterator {
    Node* current; // 当前指向的节点
    // 对于简单链表，只需要这一个指针就够了
} Iterator;

// --- 辅助函数：添加节点 (已写好，不用管) ---
void list_add(List* list, int val) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->value = val;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
}

// --- 你的任务区域 ---

// 任务 1: 创建迭代器
Iterator* iter_create(List* list) {
    Iterator* it = (Iterator*)malloc(sizeof(Iterator));
    it->current = list->head;
    return it;
}

// 任务 2: 迭代器的 next 函数
// 需求：返回当前节点，并将迭代器内部指针移向下一位
Node* iter_next(Iterator* it) {
    Node* retval_node = it->current;
    if (retval_node != NULL) {
        it->current = it->current->next;
    } else {
        return NULL;
    }
 
    return retval_node; // 占位符，请修改
}

int main() {
    // 初始化链表
    List myList = { NULL, NULL };
    list_add(&myList, 100);
    list_add(&myList, 200);
    list_add(&myList, 300);
    list_add(&myList, 400);

    printf("--- 开始遍历 ---\n");

    // 创建迭代器
    Iterator* myIter = iter_create(&myList);
    Node* node_ptr;

    while ((node_ptr = iter_next(myIter)) != NULL) {
        printf("value is: %d\n", node_ptr->value);
    }
    printf("iter over\n");

    // 清理 (简单释放迭代器即可，节点内存暂不处理)
    free(myIter);
    return 0;
}