// 2026.01.16 Yi 14:38
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct methods;
struct _iter;

typedef struct _node {
    char* key;
    int value;
    struct _node* _next;
    struct _node* _prev;
    struct _node* _left;
    struct _node* _right;
} _node;

typedef struct TM {
    int _count;
    struct _iter* _iter;
    struct _node* _head;
    struct _node* _tail;
    struct _node* _root;
    struct methods* methods;
} TM;

typedef struct _iter {
    _node* _current;
} _iter;

typedef struct methods {
    void (*TM_size)(TM* self);
    void (*TM_add)(TM* self, char* key_add, int value_add);
    void (*TM_iter)(TM* self);
    _iter* (*TM_iter_make)(TM* self);
    void (*TM_chain_dump)(TM* self);
    void (*TM_del)(TM* self);
    void (*TM_tree_dump)(TM* self);
    void (*TM_iter_del)(TM* self);
    int (*TM_get)(TM* self, char* key_target, int error_index);
} methods;

_node* _node_make(char* new_key, int new_value) {
    _node* new_node = (_node*)malloc(sizeof(*new_node));
    new_node->key = strdup(new_key);
    new_node->value = new_value;
    new_node->_next = NULL;
    new_node->_prev = NULL;
    new_node->_left = NULL;
    new_node->_right = NULL;
    return new_node;
} // 返回新节点（节点已填装数据，但没有任何连接行为）

void _node_connect(TM* o_TM, _node* curr, _node* new_node, int direction_int, _node* left_neighbor, _node* right_neighbor) {
    // dicection_int must be -1(left) / 1(right)

    if (direction_int == -1) {
        // go left
        curr->_left = new_node;

    } else if (direction_int == 1) {
        // go right
        curr->_right = new_node;
    }
    //  分三种情况：
    //      1）左右邻居都不为 NULL → new_node 夹在中间
    //      2）左邻居为 NULL → new_node 应该为头节点
    //      3）右邻居为 NULL → new_node 应该为尾节点
    // o_TM->_tail = new_node;
    // curr->_next = new_node;
    // new_node->_prev = curr;
    if (left_neighbor != NULL && right_neighbor != NULL) {
        //      1）左右邻居都不为NULL → new_node 夹在中间
        new_node->_prev = left_neighbor;
        new_node->_next = right_neighbor;
        left_neighbor->_next = new_node;
        right_neighbor->_prev = new_node;
    } else if (left_neighbor == NULL && right_neighbor != NULL) {
        //      2）左邻居为NULL → new_node 应该为头节点
        o_TM->_head = new_node;
        new_node->_next = right_neighbor;
        right_neighbor->_prev = new_node;
    } else if (left_neighbor != NULL && right_neighbor == NULL) {
        //      3）右邻居为NULL → new_node 应该为尾节点
        o_TM->_tail = new_node;
        new_node->_prev = left_neighbor;
        left_neighbor->_next = new_node;
    } else {
        // 如果程序触发了这个 else，代表一定出现了错误
        printf("错误：链表左右连接出现问题");
    }
    o_TM->_count++;
    return;
}

void _TM_size(TM* self) {
    printf("\nTM's size: %d\n", self->_count);
}

void _TM_add(TM* self, char* key_add, int value_add) {
    // 检测 TM 是否存在（是否为空）
    if (self->_root == NULL) {
        _node* new_node = _node_make(key_add, value_add);
        // 获得已填装了新数据的节点
        self->_root = new_node;
        // TM 为空，直接将新节点设置为根
        self->_count++;
        self->_head = new_node;
        self->_tail = new_node;
        // printf("_TM_add: key:%s, value:%d\n", key_add, value_add);
        return;
    } // 如果树为空 → 新节点为树根 → 任务完成

    /* - 树不为空 - */
    // 检测 key 是否已经存在于 TM 中
    _node* right_neighbor = NULL;
    _node* left_neighbor = NULL;

    _node* curr = self->_root;
    int cmp = 0;
    while (1) {
        cmp = strcmp(curr->key, key_add);
        if (cmp == 0) {
            // 已经找到了相同key的节点 → curr
            curr->value = value_add;
            // printf("_TM_add: key:%s, value:%d\n", key_add, value_add);
            return; // 直接修正节点值 → 任务完成
        } else if (cmp > 0) {
            // left
            if (curr->_left == NULL) {
                // left is NULL
                // need to create a new node on curr's left

                _node* new_node = _node_make(key_add, value_add);
                right_neighbor = curr;
                _node_connect(self, curr, new_node, -1, left_neighbor, right_neighbor); // 直接衔接新节点
                break;
            } else {
                // left is not NULL
                right_neighbor = curr;
                curr = curr->_left;
            }
        } else {
            // right
            if (curr->_right == NULL) {
                // right is NULL
                // need to create a new node on curr's right
                _node* new_node = _node_make(key_add, value_add);
                left_neighbor = curr;
                _node_connect(self, curr, new_node, 1, left_neighbor, right_neighbor); // 直接衔接新节点
                break;
            } else {
                // right is not NULL
                left_neighbor = curr;
                curr = curr->_right;
            }
        }
    }
    // printf("_TM_add: key:%s, value:%d\n", key_add, value_add);
    return;
}

void _iter_del(TM* self) {
    if (self->_iter != NULL) {
        free(self->_iter);
        self->_iter = NULL;
        printf("\n_iter_del：iter 内存已释放,\nself->iter 已赋值：NULL\n");
        return;
    } else {
        printf("\n_iter_del：iter 本身就为 NULL\n");
        return;
    }
}

_iter* _iter_make(TM* target) {
    _iter* new_iter = (_iter*)malloc(sizeof(*new_iter));

    if (target->_head != NULL) {
        new_iter->_current = target->_head;
    } else {
        // 链表根本不存在
        printf("Error：TM 链表不存在\n");
        return NULL;
    }
    // new_iter->_current = NULL;
    return new_iter;
} // 将直接返回一个 已经将当前节点锁定直链表 头 的迭代器

void _TM_iter(TM* self) {
    // 迭代器已经存在，需要获得当前curr，并将curr指向下一个
    self->_iter->_current = self->_iter->_current->_next;
}

void _TM_chain_dump(TM* self) {
    printf("\n--- chain mode dump ---\n");
    _node* curr = self->_head;
    int i = 1;
    while (curr) {
        printf("%d key: %s, value: %d\n", i, curr->key, curr->value);
        curr = curr->_next;
        i++;
        if (i == 200) {
            return;
        }
    }
    return;
}

void _ttd_script(_node* curr, int depth_index) {
    // 先打印本节点
    for (int i = 0; i < depth_index; i++) {
        printf("|   ");
    }
    printf("%s=%d\n", curr->key, curr->value);
    if (curr->_left != NULL || curr->_right != NULL) {
        if (curr->_left != NULL) {
            // 左子节点有东西
            _ttd_script(curr->_left, depth_index + 1);
        }
        if (curr->_right != NULL) {
            // 右子节点有东西
            _ttd_script(curr->_right, depth_index + 1);
        }
    }
    return;
}

void _TM_tree_dump(TM* self) {
    printf("\n--- tree mode dump ---\n");
    _node* curr = self->_root;
    int depth_index = 0;
    _ttd_script(curr, depth_index);
}

int _TM_tree_get(TM* self, char* key_target, int error_index) {
    if (self->_root == NULL) {
        // 树为空 → 直接返回，任务结束
        return error_index;
    }
    _node* curr = self->_root;
    int cmp;
    while (curr) {
        cmp = strcmp(curr->key, key_target);
        if (cmp > 0) {
            // left
            curr = curr->_left;
        } else if (cmp < 0) {
            // right
            curr = curr->_right;
        } else {
            // cmp = 0
            return curr->value;
        }
    }
    return error_index;
}

void _TM_del(TM* self) {
    _node *curr, *next;
    curr = self->_head;
    while (curr) {
        free(curr->key);
        next = curr->_next;
        free(curr);
        curr = next;
    }
    if (self->_iter != NULL) {
        free(self->_iter);
    }
    free(self->methods);
    free(self);
    printf("\nTM destroy success\n");
    return;
}

methods* methods_make() {
    methods* newmethods = (methods*)malloc(sizeof(*newmethods));
    newmethods->TM_size = &_TM_size;
    newmethods->TM_add = &_TM_add;
    newmethods->TM_iter = &_TM_iter;
    newmethods->TM_del = &_TM_del;
    newmethods->TM_chain_dump = &_TM_chain_dump;
    newmethods->TM_tree_dump = &_TM_tree_dump;
    newmethods->TM_get = &_TM_tree_get;
    newmethods->TM_iter_make = &_iter_make;
    newmethods->TM_iter_del = &_iter_del;
    return newmethods;
}

TM* TM_create() {
    TM* new_TM = (TM*)malloc(sizeof(*new_TM));
    new_TM->_count = 0;
    new_TM->_head = NULL;
    new_TM->_tail = NULL;
    new_TM->_root = NULL;
    new_TM->_iter = NULL;
    new_TM->methods = methods_make();
    return new_TM;
} // 返回一个全空的 TM 节点（方法已填装）
#ifdef TEST
int main(void) {
    TM* my_TM = TM_create();
    printf("1. Basic Insertions (h, d, b, f)\n");
    my_TM->methods->TM_add(my_TM, "h", 22);
    my_TM->methods->TM_add(my_TM, "h", 42); // Replace (更新)
    my_TM->methods->TM_add(my_TM, "d", 8);
    my_TM->methods->TM_add(my_TM, "b", 123);
    my_TM->methods->TM_add(my_TM, "f", 6);

    my_TM->methods->TM_chain_dump(my_TM);

    printf("2. More Insertions (k, m, j) - Filling gaps\n");
    my_TM->methods->TM_add(my_TM, "k", 9);
    my_TM->methods->TM_add(my_TM, "m", 67);
    my_TM->methods->TM_add(my_TM, "j", 12);

    printf("\n3. Edge Cases (Insert smaller than head, insert larger than tail)\n");
    my_TM->methods->TM_add(my_TM, "a", 1); // New Head (比 b 小)
    my_TM->methods->TM_add(my_TM, "z", 999); // New Tail (比 m 大)

    my_TM->methods->TM_size(my_TM);

    my_TM->methods->TM_chain_dump(my_TM);
    // 最终顺序: a -> b -> d -> f -> h -> j -> k -> m -> z

    my_TM->methods->TM_tree_dump(my_TM);

    printf("\n4. Iter print test\n");
    int i = 0;
    my_TM->_iter = my_TM->methods->TM_iter_make(my_TM);

    while (my_TM->_iter->_current) {
        i++;
        printf("%d key: %s, value: %d\n", i, my_TM->_iter->_current->key, my_TM->_iter->_current->value);
        my_TM->methods->TM_iter(my_TM);
    }
    my_TM->methods->TM_iter_del(my_TM->_iter);

    printf("\n5. Lookup Test\n");
    printf("Value of 'h': %d\n", my_TM->methods->TM_get(my_TM, "h", -1));
    printf("Value of 'z': %d\n", my_TM->methods->TM_get(my_TM, "z", -1));
    printf("Value of 'x' (missing): %d\n", my_TM->methods->TM_get(my_TM, "x", -1));

    printf("\n6. destroy MT\n");
    my_TM->methods->TM_del(my_TM);

    printf("\n--- main Completed ---\n");
    return 0;
}
#endif