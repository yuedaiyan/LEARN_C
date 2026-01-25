#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BUCKETS 8 // PPT Page 14 设定默认为 8

/* ==========================================
 * 数据结构定义 (Data Structures)
 * 对应 PDF 1 Page 8 & 14
 * ========================================== */

// 哈希表中的节点（条目）
struct HashMapEntry {
    char* key; // 键 (Public)
    int value; // 值 (Public)
    struct HashMapEntry* __prev; // 前驱指针 (用于链表)
    struct HashMapEntry* __next; // 后继指针 (用于链表)
};

// 哈希表主结构
struct HashMap {
    int __buckets; // 桶的数量
    int __count; // 元素总数
    struct HashMapEntry* __heads[NUM_BUCKETS]; // 头指针数组
    struct HashMapEntry* __tails[NUM_BUCKETS]; // 尾指针数组
};

// 迭代器结构 (PDF 2 Page 3)
// 这是一个"友元"类，可以直接访问 HashMap 的内部受保护数据
struct HashMapIter {
    int __bucket; // 当前遍历到的桶索引
    struct HashMap* __map; // 指向被遍历的哈希表
    struct HashMapEntry* __current; // 指向当前具体的节点
};

/* ==========================================
 * 哈希函数 (Hash Function)
 * 对应 PDF 1 Page 12
 * ========================================== */
int getBucket(char* str, int buckets) {
    unsigned int hash = 123456;
    if (str == NULL)
        return 0;
    for (; *str; str++) {
        // 经典的哈希算法：左移 + 异或
        hash = (hash << 3) ^ *str;
    }
    return hash % buckets;
}

/* ==========================================
 * HashMap 核心操作
 * ========================================== */

// 创建新的 HashMap (PDF 1 Page 14)
struct HashMap* HashMap_new() {
    struct HashMap* p = (struct HashMap*)malloc(sizeof(struct HashMap));
    p->__buckets = NUM_BUCKETS;
    p->__count = 0;
    // 初始化所有桶为 NULL
    for (int i = 0; i < p->__buckets; i++) {
        p->__heads[i] = NULL;
        p->__tails[i] = NULL;
    }
    return p;
}

// 内部查找函数 (PDF 1 Page 15)
struct HashMapEntry* __HashMap_find(struct HashMap* self, char* key, int bucket) {
    struct HashMapEntry* cur;
    if (self == NULL || key == NULL)
        return NULL;

    // 遍历指定桶的链表
    for (cur = self->__heads[bucket]; cur != NULL; cur = cur->__next) {
        if (strcmp(key, cur->key) == 0) {
            return cur;
        }
    }
    return NULL;
}

// 放入数据 (Put) (PDF 1 Page 17)
void HashMap_put(struct HashMap* self, char* key, int value) {
    int bucket = getBucket(key, self->__buckets);

    // 1. 先查找是否存在
    struct HashMapEntry* old = __HashMap_find(self, key, bucket);
    if (old != NULL) {
        old->value = value; // 存在则更新值
        return;
    }

    // 2. 不存在，创建新节点
    struct HashMapEntry* new_entry = (struct HashMapEntry*)malloc(sizeof(struct HashMapEntry));
    new_entry->key = strdup(key); // 复制字符串 key
    new_entry->value = value;
    new_entry->__next = NULL;

    // 3. 插入到链表尾部 (Tail)
    if (self->__heads[bucket] == NULL) {
        // 如果桶是空的，Head 指向新节点
        self->__heads[bucket] = new_entry;
    }
    if (self->__tails[bucket] != NULL) {
        // 如果桶不空，旧 Tail 的 next 指向新节点
        self->__tails[bucket]->__next = new_entry;
    }

    // 维护双向链表关系
    new_entry->__prev = self->__tails[bucket];
    // 更新 Tail 指针
    self->__tails[bucket] = new_entry;

    self->__count++;
}

// 获取数据 (Get) (PDF 1 Page 15)
int HashMap_get(struct HashMap* self, char* key, int def_val) {
    int bucket = getBucket(key, self->__buckets);
    struct HashMapEntry* retval = __HashMap_find(self, key, bucket);
    if (retval == NULL)
        return def_val;
    return retval->value;
}

// 打印调试信息 (Dump) (PDF 1 Page 18)
void HashMap_dump(struct HashMap* self) {
    struct HashMapEntry* cur;
    printf("Object HashMap@%p count=%d buckets=%d\n", self, self->__count, self->__buckets);
    for (int i = 0; i < self->__buckets; i++) {
        for (cur = self->__heads[i]; cur != NULL; cur = cur->__next) {
            printf(" %s=%d [%d]\n", cur->key, cur->value, i);
        }
    }
}

/* ==========================================
 * 迭代器实现 (Iterator Implementation)
 * 重点部分：对应 PDF 2 Pages 4-5
 * ========================================== */

// 迭代器构造函数 (PDF 2 Page 4)
struct HashMapIter* HashMap_iter(struct HashMap* map) {
    struct HashMapIter* iter = (struct HashMapIter*)malloc(sizeof(struct HashMapIter));
    iter->__map = map;
    iter->__bucket = 0;
    // 初始化时，指向第0个桶的头节点（可能是 NULL，这会在 next 中处理）
    iter->__current = map->__heads[iter->__bucket];
    return iter;
}

// 迭代器 Next 函数 (PDF 2 Page 5)
// 逻辑：如果当前节点为空，则扫描下一个桶，直到找到数据或所有桶扫描完毕
struct HashMapEntry* HashMapIter_next(struct HashMapIter* self) {
    struct HashMapEntry* retval;

    // 循环：如果当前链表走完了 (self->__current == NULL)，
    // 我们需要跳转到下一个非空的桶 (Bucket)
    while (self->__current == NULL) {
        self->__bucket++; // 移动到下一个桶

        // 边界检查：如果桶索引超过了最大值，说明整个 Map 遍历结束
        if (self->__bucket >= self->__map->__buckets) {
            return NULL;
        }

        // 将 current 指向新桶的 head
        self->__current = self->__map->__heads[self->__bucket];
        // 如果这个新桶也是空的 (NULL)，while 循环会继续执行，再次 bucket++
    }

    // 代码执行到这里，说明 self->__current 指向了一个有效的 Entry
    retval = self->__current;

    // 关键步骤：在返回当前值之前，先把指针移动到下一个位置，为下一次调用做准备
    if (self->__current != NULL) {
        self->__current = self->__current->__next;
    }

    return retval;
}

// 销毁迭代器
void HashMapIter_del(struct HashMapIter* self) {
    free(self);
}

/* ==========================================
 * Main 测试程序
 * ========================================== */
int main() {
    // 1. 创建 HashMap
    struct HashMap* map = HashMap_new();

    printf("--- Adding Data ---\n");
    // 这些字符串经过 getBucket 运算会落入不同的桶中 (参考 PDF 1 Page 9/12)
    HashMap_put(map, "Hi", 1);
    HashMap_put(map, "Hello", 5);
    HashMap_put(map, "World", 7);
    HashMap_put(map, "Python", 2);
    HashMap_put(map, "C", 99);

    // 2. Dump 查看内部结构
    printf("\n--- Internal Structure (Dump) ---\n");
    HashMap_dump(map);

    // 3. 使用 Iterator 遍历 (参考 PDF 2 Page 2)
    printf("\n--- Iterating ---\n");
    struct HashMapIter* iter = HashMap_iter(map);
    struct HashMapEntry* cur;

    // 循环调用 next
    while (1) {
        cur = HashMapIter_next(iter);
        if (cur == NULL)
            break; // 遍历结束
        printf("Key: %s, Value: %d\n", cur->key, cur->value);
    }

    // 4. 清理
    HashMapIter_del(iter);
    // (注：实际项目中还需要编写 HashMap_del 来释放 map 和 entry 的内存，此处略)

    return 0;
}