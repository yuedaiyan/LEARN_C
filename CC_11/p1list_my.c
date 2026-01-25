// 2026.01.20 Yi 01:12
#include <_string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Plist {
    int malloc_int;
    int size_int;
    char** list_charptr_ptr;
}Plist;

Plist* Plist_make() {
    Plist* new_Plist = (Plist*)malloc(sizeof(*new_Plist));
    new_Plist->malloc_int = 2;
    new_Plist->size_int = 0;
    new_Plist->list_charptr_ptr=(char**)malloc(new_Plist->malloc_int*sizeof(*new_Plist->list_charptr_ptr));
    return new_Plist;
}

void Plist_add(Plist* self, char* char_add) {
    if (self->size_int >= self->malloc_int) {
        printf("自动扩大数组\n");
        self->malloc_int+=2;
        self->list_charptr_ptr = (char**)realloc(self->list_charptr_ptr, self->malloc_int * sizeof(*self->list_charptr_ptr));

        for (int i = self->size_int; i < self->malloc_int; i++) {
            self->list_charptr_ptr[i]=NULL;
        }

        // self->list_charptr_ptr[self->malloc_int-2] = NULL;
        // self->list_charptr_ptr[self->malloc_int-1] = NULL;
        printf("当前已请求数组大小：%d\n",self->malloc_int);
    }

    // 拷贝数据
    // char* add_ptr = (char*)malloc(sizeof(char_add) + 1);
    // add_ptr = strdup(char_add);
    char* add_ptr = strdup(char_add);

    self->list_charptr_ptr[self->size_int] = add_ptr;
    self->size_int++;
}

void Plist_dump(Plist* self) {
    for (int i = 0; i < self->size_int; i++) {
        printf("[%d] %s\n",i,self->list_charptr_ptr[i]);
    }
}

typedef struct _node {
    char* item;
    struct _node* _next;
    struct _node* _prev;
}_node;

typedef struct Node_list {
    _node* _head;
    _node* _tail;
    int _count;
}Node_list;

Node_list* Node_list_creat() {
    Node_list* new_n_l = (Node_list*)malloc(sizeof(*new_n_l));
    new_n_l->_head = NULL;
    new_n_l->_tail = NULL;
    new_n_l->_count = 0;
    return new_n_l;
}

void Node_list_add(Node_list* self, char* char_add) {
    _node* new_node = (_node*)malloc(sizeof(*new_node));
    new_node->item = strdup(char_add);
    new_node->_next = NULL;
    if (self->_head == NULL) {
        // list is new
        self->_head = new_node;
        new_node->_prev = NULL;
    } else {
        new_node->_prev = self->_tail;
        self->_tail->_next = new_node;
    }
    self->_count++;

    self->_tail=new_node;
    // printf("new node add: \ncount:%d, char:%s\n",self->_count,new_node->item);
}

void Node_list_dump(Node_list* self) {
    _node* curr = self->_head;
    int i=0;
    while (curr) {
        printf("[%d] %s\n",i,curr->item);
        curr=curr->_next;
        i++;
    }
}

void Node_list_del(Node_list* self) {
    _node* curr = self->_head;
    _node* next = NULL;
    while (curr) {
        next = curr->_next;
        free(curr->item);
        free(curr);
        curr=next;
    }
    free(self);
}
/* ==========================================
 * 主函数：模拟 PPT 的演示流程
 * ========================================== */
int main() {
    // 1. 演示 Python 1.0 风格列表 (动态数组)
    printf("=== Demo 1: Python 1.0 List ===\n");
    Plist* my_plist=Plist_make();

    Plist_add(my_plist, "C");
    Plist_add(my_plist, "is");
    // 下一步会触发扩容 (因为初始 alloc=2, 存入第3个时需要扩容)
    Plist_add(my_plist, "Fun");

    Plist_dump(my_plist);

    // 2. 演示 K&R 风格列表 (链表)
    printf("=== Demo 2: K&R Lined List ===\n");
    Node_list* my_node_list = Node_list_creat();

    Node_list_add(my_node_list, "C");
    Node_list_add(my_node_list, "is");
    Node_list_add(my_node_list, "Fun");
    Node_list_add(my_node_list, "Fouth");
    
    printf("size:%d\n",my_node_list->_count);
    Node_list_dump(my_node_list);

    Node_list_del(my_node_list);

    return 0;
}