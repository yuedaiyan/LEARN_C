#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1000

// 1. 定义节点结构 (Node Definition)
struct Inode {
    char *text;
    struct Inode *next;
};

// 2. 定义链表容器结构 (List Container)
// 包含了头尾指针，代表一个完整的"链表对象"
struct list {
    struct Inode *head;
    struct Inode *tail;
};

// 3. 封装函数：向链表添加一行文本 [cite: 194-207]
// 参数 lst: 指向链表容器的指针
// 参数 line: 要添加的文本内容
void list_add(struct list *lst, char *line) {
    // 为新内容分配内存并复制字符串
    char *save = (char *)malloc(strlen(line) + 1);
    strcpy(save, line);

    // 创建新节点
    struct Inode *new_node = (struct Inode *)malloc(sizeof(struct Inode));
    new_node->text = save;
    new_node->next = NULL;

    // 将新节点连接到链表末尾
    if (lst->tail != NULL) {
        lst->tail->next = new_node; // 旧尾巴指向新节点
    }

    lst->tail = new_node; // 更新尾指针指向新节点

    // 如果是第一个节点，头指针也要指向它
    if (lst->head == NULL) {
        lst->head = new_node;
    }
}

int main() {
    char line[MAXLINE];

    // 声明一个链表对象，并初始化 [cite: 211-214]
    struct list mylist;
    mylist.head = NULL;
    mylist.tail = NULL;

    struct Inode *current;

    printf("请输入文本 (Ctrl+D 或 Ctrl+Z 结束):\n");

    // 读取输入并调用封装函数 [cite: 215-216]
    while (fgets(line, MAXLINE, stdin) != NULL) {
        // 去除换行符（可选优化，PPT原代码未包含此步，但推荐加上）
        line[strcspn(line, "\n")] = 0;

        list_add(&mylist, line);
    }

    printf("\n--- 输出链表内容 ---\n");

    // 遍历链表 [cite: 218]
    // 注意：这里是从 head 开始向后遍历
    for (current = mylist.head; current != NULL; current = current->next) {
        printf("%s\n", current->text);
    }

    return 0;
}