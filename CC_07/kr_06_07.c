#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 1. 必须包含此头文件以使用 strlen 和 strcpy

#define MAXLINE 1024 // 2. 定义缓冲区的大小

// 定义结构体
/*--------------------------------*/

struct lnode {
  char *text;
  struct lnode *next;
};
struct lnode *head = NULL;
struct lnode *tail = NULL;

// 根据索引删除节点 (索引从 0 开始)
/*--------------------------------*/

void delete_node_by_index(int index) {
  struct lnode *curr = head;
  struct lnode *prev = NULL;
  int count = 0;

  // 1. 没有任何节点的情况
  if (head == NULL) {
    printf("List is empty, nothing to delete.\n");
    return;
  }

  // 2. 遍历链表寻找目标节点
  while (curr != NULL && count != index) {
    prev = curr;       // 记录当前节点为“上一个”
    curr = curr->next; // 移动到下一个
    count++;
  }

  // 3. 如果 curr 为 NULL，说明索引超出了链表长度
  if (curr == NULL) {
    printf("Index %d out of bounds.\n", index);
    return;
  }

  // --- 找到了目标节点 curr，开始删除 ---

  // 情况 A: 删除的是头节点 (Head)
  if (prev == NULL) {
    head = curr->next;
    // 如果链表删空了，tail 也要置空
    if (head == NULL) {
      tail = NULL;
    }
  }
  // 情况 B: 删除的是中间节点 或 尾节点
  else {
    prev->next = curr->next;

    // 特殊处理：如果删除的是尾节点 (tail)，需要更新 tail 指针
    if (curr->next == NULL) {
      tail = prev;
    }
  }

  // 4. 释放内存 (非常重要！)
  printf("Deleting node index %d: %s", index, curr->text); // 打印一下被删的内容
  free(curr->text); // 先释放字符串内容的内存
  free(curr);       // 再释放节点本身的内存
}

// 主函数启动
/*--------------------------------*/

int main() {
  printf("main stat\n");
  char line[MAXLINE]; // 3. 定义用于存储输入行的字符数组

  printf("please input (first time): \n");
  // 使用 fgets 读取标准输入
  while (fgets(line, MAXLINE, stdin) != NULL) {
    // 4. 为字符串分配内存
    printf("please input: \n");
    char *save = (char *)malloc(strlen(line) + 1);
    if (save == NULL)
      return 1; // 内存分配检查
    strcpy(save, line);

    // 5. 为新节点分配内存
    // 注意：在 C 中尽量避免使用 'new' 作为变量名，因为它在 C++ 中是关键字
    struct lnode *new_node = (struct lnode *)malloc(sizeof(struct lnode));
    if (new_node == NULL)
      return 1;

    new_node->text = save;
    new_node->next = NULL;

    // 6. 维护链表逻辑
    if (tail != NULL) {
      tail->next = new_node;
    }
    tail = new_node;

    if (head == NULL) {
      head = new_node;
    }
  }
  printf("while end\n");

  // === 这里演示删除操作 ===
  // 假设你想删除第 2 行（索引为 1，因为从 0 开始算）
  // 只有当你输入的行数足够多时才会有效果
  printf("\n-- Attempting to delete the 2nd line (index 1) --\n");
  delete_node_by_index(1);

  printf("\n-- Start print out (After Deletion) --\n");
  struct lnode *curr = head;
  int i = 0;
  while (curr != NULL) {
    printf("[%d]: %s", i++, curr->text);
    curr = curr->next;
  }

  printf("--start print out--\n");
  // --- 测试输出 (可选) ---
  // struct lnode *curr = head;
  while (curr != NULL) {
    printf("%s", curr->text);
    curr = curr->next;
  }

  printf("--free mem--\n");
  // 7. 释放内存 (良好的编程习惯)
  curr = head;
  while (curr != NULL) {
    struct lnode *temp = curr;
    curr = curr->next;
    free(temp->text);
    free(temp);
  }

  return 0;
}