#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
 * ============================================================
 * 知识点 1: 核心数据结构 (对应 PPT Page 3)
 * TreeMapEntry 代表树上的一个节点
 * TreeMap 代表整棵树的入口
 * ============================================================
 */

struct TreeMapEntry {
    char *key;                  /* 键：字符串 */
    int value;                  /* 值：整数 */
    struct TreeMapEntry *__left;  /* 左孩子指针：存比当前key小的 */
    struct TreeMapEntry *__right; /* 右孩子指针：存比当前key大的 */
};

struct TreeMap {
    struct TreeMapEntry *__root;  /* 根节点指针 */
    int __count;                  /* 节点总数 (PPT提及但未展开，这里保留) */
};

/* 辅助函数：创建一个新节点 (PPT中省略了 malloc 的细节) */
struct TreeMapEntry* TreeMapEntry_new(char *key, int value) {
    struct TreeMapEntry *entry = malloc(sizeof(struct TreeMapEntry));
    if (entry == NULL) return NULL; // 内存分配失败
    
    // strdup 会自动 malloc 内存并复制字符串，防止外部修改 key
    entry->key = strdup(key); 
    entry->value = value;
    entry->__left = NULL;
    entry->__right = NULL;
    return entry;
}

/* 构造函数：创建一棵空树 */
struct TreeMap* TreeMap_new() {
    struct TreeMap *map = malloc(sizeof(struct TreeMap));
    map->__root = NULL;
    map->__count = 0;
    return map;
}

/* 
 * ============================================================
 * 知识点 2: 插入数据 (对应 PPT Page 9)
 * 逻辑：
 * 1. 如果树是空的，直接把新节点当根。
 * 2. 否则，通过 while 循环向下寻找。
 * 3. 比较字符串 (strcmp):
 *    - 相等 (0): 更新 value (Replace)
 *    - 小于 (<0): 向左走
 *    - 大于 (>0): 向右走
 * ============================================================
 */
void TreeMap_put(struct TreeMap* self, char *key, int value) {
    // 情况 1: 空树，直接成为根节点
    if (self->__root == NULL) {
        self->__root = TreeMapEntry_new(key, value);
        self->__count++;
        return;
    }

    struct TreeMapEntry *cur = self->__root;
    int cmp;

    while (cur != NULL) {
        cmp = strcmp(key, cur->key);

        // 情况 2: Key 存在，更新 Value (Replace)
        if (cmp == 0) {
            cur->value = value;
            return; 
        }

        // 情况 3: Key 比当前节点小，往左走
        if (cmp < 0) {
            // 如果左边是空的，就插入在这里
            if (cur->__left == NULL) {
                cur->__left = TreeMapEntry_new(key, value);
                self->__count++;
                return;
            }
            // 否则继续往左找
            cur = cur->__left;
        } 
        // 情况 4: Key 比当前节点大，往右走
        else {
            // 如果右边是空的，就插入在这里
            if (cur->__right == NULL) {
                cur->__right = TreeMapEntry_new(key, value);
                self->__count++;
                return;
            }
            // 否则继续往右找
            cur = cur->__right;
        }
    }
}

/* 
 * ============================================================
 * 知识点 3: 查找数据 (对应 PPT Page 12)
 * 逻辑与 put 类似，但找到后返回 value，没找到返回默认值(def)
 * ============================================================
 */
int TreeMap_get(struct TreeMap* self, char *key, int def) {
    struct TreeMapEntry *cur = self->__root;
    int cmp;

    while (cur != NULL) {
        cmp = strcmp(key, cur->key);

        if (cmp == 0) return cur->value;      // 找到了
        else if (cmp < 0) cur = cur->__left;  // 往左找
        else cur = cur->__right;              // 往右找
    }
    return def; // 没找到
}

/* 
 * ============================================================
 * 知识点 4: 递归遍历/打印树 (对应 PPT Page 11)
 * 这是一个深度优先遍历 (DFS)
 * depth 参数用于控制缩进，展示树的层级结构
 * ============================================================
 */
void __TreeMap_dump_recursive(struct TreeMapEntry *cur, int depth) {
    if (cur == NULL) return;

    // 1. 打印竖线缩进
    for (int i = 0; i < depth; i++) printf("| ");
    
    // 2. 打印当前节点
    printf("%s=%d\n", cur->key, cur->value);

    // 3. 递归打印左子树 (depth + 1)
    if (cur->__left != NULL) {
        __TreeMap_dump_recursive(cur->__left, depth + 1);
    }

    // 4. 递归打印右子树 (depth + 1)
    if (cur->__right != NULL) {
        __TreeMap_dump_recursive(cur->__right, depth + 1);
    }
}

/* 对外的 dump 接口 */
void TreeMap_dump(struct TreeMap* self) {
    printf("Object TreeMap count=%d\n", self->__count);
    __TreeMap_dump_recursive(self->__root, 0);
    printf("-----------------------------\n");
}

/* 
 * 额外补充: 内存回收 (Good Practice)
 * PPT没讲这个，但写C语言必须要在程序结束时释放内存
 */
void __TreeMap_free_recursive(struct TreeMapEntry *cur) {
    if (cur == NULL) return;
    __TreeMap_free_recursive(cur->__left);
    __TreeMap_free_recursive(cur->__right);
    free(cur->key); // 释放 strdup 分配的字符串
    free(cur);      // 释放节点本身
}

void TreeMap_destroy(struct TreeMap* self) {
    __TreeMap_free_recursive(self->__root);
    free(self);
}

/* 
 * ============================================================
 * 主函数 (对应 PPT Page 10)
 * 模拟 PPT 中的测试用例
 * ============================================================
 */
int main(void) {
    struct TreeMap *map = TreeMap_new();

    // 第一阶段插入
    printf("--- Phase 1 Insertions ---\n");
    TreeMap_put(map, "h", 22);
    TreeMap_put(map, "h", 42); // Replace (更新)
    TreeMap_put(map, "d", 8);
    TreeMap_put(map, "b", 123);
    TreeMap_put(map, "f", 6);

    // 打印状态
    TreeMap_dump(map);

    // 第二阶段插入
    printf("--- Phase 2 Insertions ---\n");
    TreeMap_put(map, "k", 9);
    TreeMap_put(map, "m", 67);
    TreeMap_put(map, "j", 12);

    // 打印状态
    TreeMap_dump(map);

    // 测试 Get 功能
    printf("--- Testing Get ---\n");
    printf("Get 'h': %d (Expected: 42)\n", TreeMap_get(map, "h", -1));
    printf("Get 'b': %d (Expected: 123)\n", TreeMap_get(map, "b", -1));
    printf("Get 'z': %d (Expected: -1)\n", TreeMap_get(map, "z", -1));

    // 释放内存
    TreeMap_destroy(map);

    return 0;
}