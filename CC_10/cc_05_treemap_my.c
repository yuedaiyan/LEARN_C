// 2026.01.15 Yi 17:18
#include <_string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/syslimits.h>

typedef struct __node {
    char* key;
    int value;
    struct __node* __left;
    struct __node* __right;
} __node;

typedef struct Tree {
    struct __node* root;
    int count;
} Tree;

Tree* Tree_create() {
    // printf("func Tree_creat has been call\n");
    Tree* ret_tree = (Tree*)malloc(sizeof(*ret_tree));
    ret_tree->root = NULL;
    ret_tree->count = 0;
    return ret_tree;
} // 创建 树
__node* make_node(char* key_add, int value_add) {
    // printf("func make_node has been call\n");
    __node* new_node = (__node*)malloc(sizeof(*new_node));
    new_node->key = strdup(key_add);
    new_node->value = value_add;
    new_node->__left = NULL;
    new_node->__right = NULL;
    return new_node;
} // make_node()函数功能：返回一个“已填装数据”，但“未进行任何连接”的节点

void Tree_add(Tree* self, char* key_add, int value_add) {
    // printf("func Tree_add has been call\n");
    // printf("    new add key:%s, value:%d\n",key_add,value_add);
    // 判断self是不是新树，即root是不是NULL
    if (self->root == NULL) {
        // printf("    new root\n");
        // self是新树，直接设置root节点即可
        self->root = make_node(key_add, value_add);
        self->count++;
        // printf("func Tree_add has over(root)\n");
        // printf("    new node: key:%s, value:%d\n", self->root->key, self->root->value);
        return;
    }

    // 如果self不为空，即node需要按规则找到NULL，并自动衔接
    // printf("    add node on tree\n");

    __node* tree_curr_node = self->root;
    int cmp = strcmp(tree_curr_node->key, key_add);

    if (cmp == 0) {
        // 检测如果根节点相同
        tree_curr_node->value = value_add;
        return;
    }
    // 现在，cpm肯定不为0，必须进入树内部搜寻
    while (cmp != 0) {
        // cmp!=0 意味着未找到相同key
        if (cmp == 0) {
            // 等于0，此节点就是目标节点
            tree_curr_node->value = value_add;
            break; // 将目标节点返回
        } else if (cmp > 0) {
            if (tree_curr_node->__left == NULL) {
                // 提前检测左侧是否为空
                // 如果为空,直接衔接数据填装节点，并自动连接
                tree_curr_node->__left = make_node(key_add, value_add);
                // printf("    new node: key:%s, value:%d\n", tree_curr_node->__left->key, tree_curr_node->__left->value);
                break;
            }
            // 如果左侧不为空，进入左侧节点
            tree_curr_node = tree_curr_node->__left;
            // while()一轮结束，tree_curr_node刷新，进入下一轮节点
            cmp = strcmp(tree_curr_node->key, key_add);
        } else {
            // ( cmp < 0 )
            // 大于0，向右衔接
            if (tree_curr_node->__right == NULL) {
                // 提前检测左侧是否为空
                // 如果为空,直接衔接数据填装节点，并自动连接
                tree_curr_node->__right = make_node(key_add, value_add);
                // printf("    new node: key:%s, value:%d\n", tree_curr_node->__right->key, tree_curr_node->__right->value);
                break;
            }
            // 如果左侧不为空，进入左侧节点
            tree_curr_node = tree_curr_node->__right;
            // while()一轮结束，tree_curr_node刷新，进入下一轮节点
            cmp = strcmp(tree_curr_node->key, key_add);
        }
    }
    // 到了这里，代表新node已经自动连接，数据已填装
    self->count++;
    // printf("func Tree_add has over\n");
    // printf("map's count: %d\n",self->count);
    return;
} // 往树中添加节点

void __printf_tree_node(__node* curr_node, int depth) {
    if (curr_node == NULL) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        printf("|   ");
    }
    printf("key: %s, value: %d\n", curr_node->key, curr_node->value);

    if (curr_node->__left != NULL) {
        __printf_tree_node(curr_node->__left, depth + 1);
    }

    if (curr_node->__right != NULL) {
        __printf_tree_node(curr_node->__right, depth + 1);
    }
} // 打印树 函数 专用子函数

void Tree_dump(Tree* self) {
    // printf("\nfunc Tree_dump has been call\n");
    if (self->root == NULL) {
        printf("target tree is empty\n");
        return;
    } // 检测树是不是空的

    // 现在树肯定不为空
    int depth = 0;
    // printf("\n");
    __printf_tree_node(self->root, depth);
} // 自动打印树

int Tree_get(Tree* self, char* key_get, int error_index) {
    int retint = error_index;
    if (self->root == NULL) {
        return retint;
    } // 整个树为空

    __node* curr_node = self->root;
    int cmp = strcmp(curr_node->key, key_get);
    // 已经锁定至树根

    while (1) {
        if (curr_node == NULL) {
            // 树已经到了尽头
            break;
        }
        cmp = strcmp(curr_node->key, key_get);
        if (cmp == 0) {
            retint = curr_node->value;
            break;
        } else if (cmp > 0) {
            curr_node = curr_node->__left;
        } else if (cmp < 0) {
            curr_node = curr_node->__right;
        } else {
            break;
        }
    } // 等于0之后，直接宣布找到，跳出循环
    return retint;
} // 获得特定 key 数据

void __tree_del(__node* curr) {
    if (curr == NULL) {
        return;
    }

    if (curr->__left != NULL) {
        __tree_del(curr->__left);
    }
    if (curr->__right != NULL) {
        __tree_del(curr->__right);
    }
    free(curr->key);
    free(curr);
    return;
} // 释放内存函数的专用子函数

void Tree_del(Tree* self) {
    if (self->root == NULL) {
        printf("the Tree is already empty\n");
        return;
    }
    // 现在 Tree 肯定不是空的了
    __node* curr_node = self->root;
    __tree_del(curr_node);
    free(self);
    printf("func del run over\n");
    return;
} // 用于释放Tree的内存

void try_01(Tree* self) {
    // 第一阶段插入
    printf("--- Phase 1 Insertions ---\n");
    Tree_add(self, "h", 22);

    Tree_add(self, "d", 8);
    Tree_add(self, "b", 123);
    Tree_add(self, "f", 6);

    // 打印状态
    Tree_dump(self);

    // 第二阶段插入
    printf("--- Phase 2 Insertions ---\n");
    Tree_add(self, "h", 42); // Replace (更新)
    Tree_add(self, "k", 9);
    Tree_add(self, "m", 67);
    Tree_add(self, "j", 12);

    // 打印状态
    Tree_dump(self);
} // 测试数据 1

void try_02(Tree* self) {
    // 第一阶段插入
    printf("\n--- Phase 1 Insertions ---\n");
    Tree_add(self, "a", 10);
    Tree_add(self, "b", 20);
    Tree_add(self, "c", 30);
    Tree_add(self, "d", 40);
    Tree_add(self, "e", 50);
    Tree_add(self, "f", 60);
    Tree_add(self, "g", 70);
    Tree_add(self, "h", 80);
    Tree_add(self, "i", 90);
    Tree_add(self, "j", 100);
    Tree_add(self, "k", 110);
    Tree_add(self, "l", 120);
    Tree_add(self, "m", 130);
    Tree_add(self, "n", 140);
    Tree_add(self, "o", 150);
    Tree_add(self, "p", 160);

    Tree_dump(self);

    Tree_add(self, "banana", 120);
    Tree_add(self, "apple", 130);
    Tree_add(self, "computer", 150);
    Tree_add(self, "python", 160);
    Tree_add(self, "ass", 120);
    Tree_add(self, "fuck", 130);
    Tree_add(self, "shit", 140);
    Tree_add(self, "mouse", 150);
    Tree_add(self, "feet", 160);

    Tree_dump(self);
} // 测试数据 2

// 测试主函数
int main(void) {
    // printf("\n");
    Tree* my_Tree = Tree_create();

    try_01(my_Tree);

    // try_02(my_Tree);

    // 测试 Get 功能
    printf("--- Testing Get ---\n");
    printf("Get 'h': %d (Expected: 42)\n", Tree_get(my_Tree, "h", -1));
    printf("Get 'b': %d (Expected: 123)\n", Tree_get(my_Tree, "b", -1));
    printf("Get 'z': %d (Expected: -1)\n", Tree_get(my_Tree, "z", -1));

    // 释放内存
    Tree_del(my_Tree);

    return 0;
}