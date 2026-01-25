#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==========================================
 * 数据结构定义 (对应 PDF 2 Page 5)
 * ========================================== */

// 树的节点，同时也是链表的节点
typedef struct TreeMapEntry {
    char* key; // 键
    int value; // 值
    struct TreeMapEntry* __next; // 【链表】指向排序后的下一个节点 (LinkedList next)
    struct TreeMapEntry* __left; // 【 树 】左子节点 (Binary Tree left)
    struct TreeMapEntry* __right; // 【 树 】右子节点 (Binary Tree right)
} TreeMapEntry;

// LinkedTreeMap 主结构
typedef struct TreeMap {
    TreeMapEntry* __head; // 【链表】指向链表头部 (最小值)
    TreeMapEntry* __root; // 【 树 】指向树的根节点
    int __count; // 元素计数
} TreeMap;

/* ==========================================
 * 辅助函数：创建新节点
 * ========================================== */
TreeMapEntry* TreeMapEntry_new(char* key, int value) {
    TreeMapEntry* new_node = (TreeMapEntry*)malloc(sizeof(TreeMapEntry));
    if (new_node) {
        // 复制 key 字符串，防止外部修改影响内部
        new_node->key = strdup(key);
        new_node->value = value;
        new_node->__next = NULL;
        new_node->__left = NULL;
        new_node->__right = NULL;
    }
    return new_node;
}

/* ==========================================
 * 构造函数
 * ========================================== */
TreeMap* TreeMap_new() {
    TreeMap* map = (TreeMap*)malloc(sizeof(TreeMap));
    if (map) {
        map->__head = NULL;
        map->__root = NULL;
        map->__count = 0;
    }
    return map;
}

/* ==========================================
 * 核心功能：PUT (插入或更新)
 * 对应 PDF 2 Pages 10 - 20 的核心逻辑
 * ========================================== */
void TreeMap_put(TreeMap* self, char* key, int value) {
    // 1. 处理空树的情况 (PDF 2 Page 10)
    if (self->__root == NULL) {
        TreeMapEntry* new_node = TreeMapEntry_new(key, value);
        self->__root = new_node;
        self->__head = new_node; // 链表头也是它
        self->__count++;
        return;
    }

    /*
     * 初始化搜索指针
     * cur: 当前遍历到的节点
     * left (predecessor): 路径上遇到的、比目标key小的最近节点 (即链表中的前驱)
     * right (successor):  路径上遇到的、比目标key大的最近节点 (即链表中的后继)
     *
     * 这一步对应 PDF 2 Page 12 "Finding an Item or Gap"
     */
    TreeMapEntry* cur = self->__root;
    TreeMapEntry* left_neighbor = NULL;
    TreeMapEntry* right_neighbor = NULL;
    int cmp;

    // 2. 遍历树，寻找插入位置，同时确定链表的左右邻居
    while (cur != NULL) {
        cmp = strcmp(key, cur->key);

        if (cmp == 0) {
            // 找到相同的 key，直接更新 value (PDF 2 Page 36)
            cur->value = value;
            return;
        }

        if (cmp < 0) {
            // 目标比当前小，向左走
            // 当前节点变成我们在链表中的“右侧邻居” (Successor)
            right_neighbor = cur;
            cur = cur->__left;
        } else {
            // 目标比当前大，向右走
            // 当前节点变成我们在链表中的“左侧邻居” (Predecessor)
            left_neighbor = cur;
            cur = cur->__right;
        }
    }

    // 此时 cur == NULL，我们要插入新节点
    TreeMapEntry* new_node = TreeMapEntry_new(key, value);
    self->__count++;

    /*
     * 3. 插入到链表 (Linked List Insert)
     * 对应 PDF 2 Page 16, 19, 25
     *
     * 逻辑：
     * new_node 应该被夹在 left_neighbor 和 right_neighbor 之间。
     * 顺序是: left_neighbor -> new_node -> right_neighbor
     */

    // 连接新节点的 next 指针指向右邻居
    new_node->__next = right_neighbor;

    // 连接左邻居的 next 指针指向新节点
    if (left_neighbor != NULL) {
        left_neighbor->__next = new_node;
    } else {
        // 如果没有左邻居，说明新节点比所有已知节点都小，它变成了新的头节点 (Head)
        self->__head = new_node;
    }

    /*
     * 4. 插入到树 (Tree Insert)
     * 对应 PDF 2 Page 17, 20, 26, 32
     *
     * 逻辑：
     * 新节点的父节点必然是 left_neighbor 或者 right_neighbor 中的一个。
     * 且一定是那个“还有空余子节点插槽”的节点。
     */

    // 尝试挂在右邻居的左边 (因为我们是比右邻居小的)
    // 检查：如果右邻居存在，且右邻居的左孩子是空的
    if (right_neighbor != NULL && right_neighbor->__left == NULL) {
        right_neighbor->__left = new_node;
    }
    // 否则，挂在左邻居的右边 (因为我们是比左邻居大的)
    // 检查：如果左邻居存在，且左邻居的右孩子是空的
    else if (left_neighbor != NULL && left_neighbor->__right == NULL) {
        left_neighbor->__right = new_node;
    } else {
        // 理论上不可能走到这里，除非树结构被破坏
        printf("Error: Logic failure in TreeMap_put\n");
        free(new_node->key);
        free(new_node);
    }
}

/* ==========================================
 * GET (查找)
 * 对应 PDF 1 Page 12
 * ========================================== */
int TreeMap_get(TreeMap* self, char* key, int default_val) {
    TreeMapEntry* cur = self->__root;
    int cmp;

    while (cur != NULL) {
        cmp = strcmp(key, cur->key);
        if (cmp == 0)
            return cur->value;
        else if (cmp < 0)
            cur = cur->__left;
        else
            cur = cur->__right;
    }
    return default_val;
}

/* ==========================================
 * 遍历：基于链表 (Iterate via List)
 * 这是 LinkedTreeMap 最大的优势，按序遍历只需 O(N)
 * ========================================== */
void TreeMap_print_list(TreeMap* self) {
    printf("--- List View (Ordered) ---\n");
    TreeMapEntry* cur = self->__head; // 从链表头开始
    while (cur != NULL) {
        printf("%s = %d -> ", cur->key, cur->value);
        cur = cur->__next; // 沿着 next 指针走
    }
    printf("NULL\n");
}

/* ==========================================
 * 调试：基于树的递归打印 (Tree Dump)
 * 用于验证树结构是否正确
 * ========================================== */
void _dump_tree_recursive(TreeMapEntry* node, int depth) {
    if (node == NULL)
        return;

    // 为了可视化，简单打印缩进
    for (int i = 0; i < depth; i++)
        printf("|  ");
    printf("%s=%d\n", node->key, node->value);

    if (node->__left)
        _dump_tree_recursive(node->__left, depth + 1);
    if (node->__right)
        _dump_tree_recursive(node->__right, depth + 1);
}

void TreeMap_dump_tree(TreeMap* self) {
    printf("--- Tree View (Structure) ---\n");
    if (self->__root == NULL)
        printf("(empty)\n");
    else
        _dump_tree_recursive(self->__root, 0);
}

/* ==========================================
 * Main 测试函数
 * 复现 PPT 中的测试用例
 * ========================================== */
int main(void) {
    TreeMap* map = TreeMap_new();

    printf("1. Basic Insertions (h, d, b, f)\n");
    TreeMap_put(map, "h", 22);
    TreeMap_put(map, "h", 42); // Replace (更新)
    TreeMap_put(map, "d", 8);
    TreeMap_put(map, "b", 123);
    TreeMap_put(map, "f", 6);

    TreeMap_print_list(map); // 应该输出: b=123 -> d=8 -> f=6 -> h=42 -> NULL
    TreeMap_dump_tree(map);
    printf("\n");

    printf("2. More Insertions (k, m, j) - Filling gaps\n");
    TreeMap_put(map, "k", 9);
    TreeMap_put(map, "m", 67);
    TreeMap_put(map, "j", 12);

    TreeMap_print_list(map); // 顺序应该是: b, d, f, h, j, k, m

    printf("\n3. Edge Cases (Insert smaller than head, insert larger than tail)\n");
    TreeMap_put(map, "a", 1); // New Head (比 b 小)
    TreeMap_put(map, "z", 999); // New Tail (比 m 大)

    TreeMap_print_list(map);
    // 最终顺序: a -> b -> d -> f -> h -> j -> k -> m -> z
    TreeMap_dump_tree(map);
    printf("\n4. Lookup Test\n");
    printf("Value of 'h': %d\n", TreeMap_get(map, "h", -1));
    printf("Value of 'z': %d\n", TreeMap_get(map, "z", -1));
    printf("Value of 'x' (missing): %d\n", TreeMap_get(map, "x", -1));

    // 这里省略了内存释放函数，实际使用中需要编写 destroy 函数遍历链表释放内存
    return 0;
}