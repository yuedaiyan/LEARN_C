/*
题目场景：办公室打印机大作战
你正在为公司的网络打印机编写驱动程序。打印机有一个任务队列（链表），任务按顺序排列。你需要实现三个核心功能：

普通打印：员工提交文档，任务排到队伍的最后面。
老板插队 (VIP)：老板很急，他的文档必须直接插入到队伍的最前面（成为新的 Head）。
取消任务：如果有员工发错了文件，需要根据任务 ID 把那个任务从队列中删掉。
你的任务
请基于下方的基础代码，补充完整三个函数的逻辑：

add_to_tail (普通打印)
insert_at_head (老板插队)
delete_by_id (取消任务)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 打印任务结构体
typedef struct PrintJob {
    int id;         // 任务ID (唯一)
    char *doc_name; // 文档名
    struct PrintJob *next;
} PrintJob;

PrintJob *head = NULL;
PrintJob *tail = NULL;

// 辅助函数：创建一个新节点 (帮你写好了，专注链表逻辑)
PrintJob *create_job(int id, const char *name) {
    PrintJob *new_job =
        (PrintJob *)malloc(sizeof(PrintJob)); // 新建了一个Printjob结构体
    new_job->id = id;                         // 往结构体里传入内容
    new_job->doc_name = (char *)malloc(
        strlen(name) + 1); // 结构体内的内容指针被建立,根文件指针刚好一样大
    strcpy(new_job->doc_name, name); // 文件指针被放入结构体
    new_job->next = NULL;            // 只有一个任务,下一个指针为空
    return new_job;                  // 返回刚刚建立好的结构体
    // 返回的结构体中,有一个int id,还有一个*doc_name,*next是NULL
}

// ==========================================
// ⬇️ 请在这个区域编写你的代码 ⬇️
// ==========================================

// 1. 普通打印：添加到队尾
// add_to_tail(101, "Report.pdf");
// add_to_tail(102, "Budget.xlsx");
// add_to_tail(103, "Meme.jpg");
void add_to_tail(int id, const char *name) {

    PrintJob *new_job = create_job(id, name);
    if (head == NULL) {
        head = new_job;
        tail = new_job;
        return;
    }
    tail->next = new_job;
    tail = new_job;
    return;

    // 提示：要考虑链表目前是否为空的情况
}

// 2. 老板插队：添加到队头
void insert_at_head(int id, const char *name) {
    PrintJob *new_job = create_job(id, name);
    if (head == NULL) {
        head = new_job;
        tail = new_job;
        return;
    }
    new_job->next = head;
    head = new_job;
    // 警告：如果链表之前是空的，插队进去的这个节点既是头也是尾，记得更新 tail！
}

// 3. 取消任务：根据 ID 删除节点
void delete_by_id(int target_id) {
    // 如果链表为空
    if (head == NULL) {
        return;
    }
    // 如果删除的是头部节点
    if (head != NULL && target_id == head->id) {
        PrintJob *temp = head;
        head = head->next;
        free(temp->doc_name);
        free(temp);
        return;
    }

    PrintJob *prev_ptr = NULL;
    PrintJob *curr_ptr = head;

    // Find it!
    while (curr_ptr != NULL && curr_ptr->id != target_id) {
        prev_ptr = curr_ptr;
        curr_ptr = curr_ptr->next;
    }

    // 如果删除的是尾部节点
    if (target_id == tail->id) {
        prev_ptr->next = NULL;
        tail = prev_ptr;
        free(curr_ptr->doc_name);
        free(curr_ptr);
        return;
    }

    // 如果是删除的是中间节点
    if (target_id != tail->id) {
        prev_ptr->next = curr_ptr->next;
        free(curr_ptr->doc_name);
        free(curr_ptr);
        return;
    }
    return;
    // 提示：
    // 1. 链表为空直接返回
    // 2. 如果删除的是头节点？
    // 3. 如果删除的是中间节点？
    // 4. 如果删除的是尾节点 (tail 需要更新吗？)
    // 5. 记得 free 内存 (doc_name 和 节点本身)
}

// ==========================================
// ⬆️ 你的代码结束 ⬆️
// ==========================================

// 打印当前队列 (用于测试)
void print_queue() {
    PrintJob *curr = head;
    printf("\n--- Current Printer Queue ---\n");
    if (curr == NULL) {
        printf("(Queue is empty)\n");
        return;
    }
    while (curr != NULL) {
        printf("[ID: %d] %s", curr->id, curr->doc_name);
        if (curr == head)
            printf(" (Printing Now)");
        if (curr == tail)
            printf(" (Last)");
        printf("\n");
        curr = curr->next;
    }
    printf("-----------------------------\n");
}

int main() {
    // 模拟测试场景
    printf("1. Adding 3 normal jobs...\n");
    add_to_tail(101, "Report.pdf");
    add_to_tail(102, "Budget.xlsx");
    add_to_tail(103, "Meme.jpg");
    print_queue();

    printf("\n2. BOSS IS COMING! Emergency insert...\n");
    insert_at_head(999, "URGENT_BOSS.ppt");
    print_queue();

    printf("\n3. Canceling job 102 (Middle)...\n");
    delete_by_id(102);
    print_queue();

    printf("\n4. Canceling job 103 (The Tail)...\n");
    delete_by_id(103);
    print_queue(); // 此时 tail 应该指向 101

    printf("\n5. Job 999 finished (Delete Head)...\n");
    delete_by_id(999);
    print_queue();

    // 最后的清理内存代码略，重点是上面三个函数
    return 0;
}