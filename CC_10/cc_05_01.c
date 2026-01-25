#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义桶的大小，PPT中固定为8
#define BUCKETS 8

// ==========================================
// 1. 结构体定义 (参考第 8 和 14 页)
// ==========================================

// 前置声明，为了在函数指针中使用
struct HashMap;

// 链表节点：存储键值对
struct HashMapEntry {
    char* key; // 键
    int value; // 值
    struct HashMapEntry* __prev; // 前驱指针
    struct HashMapEntry* __next; // 后继指针
};

// 哈希表容器
struct HashMap {
    int __buckets;
    struct HashMapEntry* __heads[BUCKETS]; // 存放每个桶的头指针
    struct HashMapEntry* __tails[BUCKETS]; // 存放每个桶的尾指针
    int __count; // 记录总元素个数

    // 函数指针：模拟面向对象的方法
    void (*put)(struct HashMap* self, char* key, int value);
    int (*get)(struct HashMap* self, char* key, int def);
    void (*dump)(struct HashMap* self);
    // PPT中提到了 iter 和 del，但未给出完整代码，此处暂略
};

// ==========================================
// 2. 哈希函数 (参考第 12 页)
// ==========================================
int getBucket(char* str, int buckets) {
    unsigned int hash = 123456;
    if (str == NULL)
        return 0;
    for (; *str; str++) {
        // 移位和异或操作，简单的哈希算法
        hash = (hash << 3) ^ *str;
    }
    return hash % buckets;
}

// ==========================================
// 3. 核心功能实现
// ==========================================

// 内部查找函数 (参考第 15 页)
struct HashMapEntry* __HashMap_find(struct HashMap* self, char* key, int bucket) {
    struct HashMapEntry* cur;
    if (self == NULL || key == NULL)
        return NULL;

    // 只遍历对应的那个桶
    for (cur = self->__heads[bucket]; cur != NULL; cur = cur->__next) {
        if (strcmp(key, cur->key) == 0)
            return cur;
    }
    return NULL;
}

// 获取值 (参考第 15 页)
int __HashMap_get(struct HashMap* self, char* key, int def) {
    int bucket = getBucket(key, self->__buckets);
    struct HashMapEntry* retval = __HashMap_find(self, key, bucket);// 开始查找
    if (retval == NULL)
        return def;
    return retval->value;
}

// 插入或更新值 (参考第 17 页)
void __HashMap_put(struct HashMap* self, char* key, int value) {
    int bucket;
    struct HashMapEntry *old, *new_entry;

    // 1. 确定桶的位置
    bucket = getBucket(key, self->__buckets);
    // 这里返回了一个0-7的哈希值

    // 2. 检查key是否已存在，如果存在则更新
    old = __HashMap_find(self, key, bucket);
    if (old != NULL) {
        old->value = value;
        return;
    }

    // 3. 如果不存在，创建新节点
    new_entry = malloc(sizeof(*new_entry));
    // 注意：实际工程中这里需要复制key字符串 (strdup)，防止外部修改
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->__next = NULL;

    // 4. 插入到对应桶的链表末尾
    // 如果桶是空的
    if (self->__heads[bucket] == NULL) {
        self->__heads[bucket] = new_entry;
    }

    // 如果桶里已经有东西，把新节点接在尾部
    if (self->__tails[bucket] != NULL) {
        self->__tails[bucket]->__next = new_entry;
    }

    // 维护双向链表关系
    new_entry->__prev = self->__tails[bucket];
    self->__tails[bucket] = new_entry;

    // 计数器+1
    self->__count++;
}

// 打印哈希表详情 (参考第 18 页)
void __HashMap_dump(struct HashMap* self) {
    int i;
    struct HashMapEntry* cur;

    printf("\nObject HashMap@%p count=%d buckets=%d\n",
        self, self->__count, self->__buckets);

    // 外层循环：遍历桶
    for (i = 0; i < self->__buckets; i++) {
        // 内层循环：遍历链表
        for (cur = self->__heads[i]; cur != NULL; cur = cur->__next) {
            printf(" %s=%d [%d]\n", cur->key, cur->value, i);
        }
    }
}

// ==========================================
// 4. 构造函数 (参考第 14 页)
// ==========================================
struct HashMap* HashMap_new() {
    struct HashMap* p = malloc(sizeof(*p));

    p->__buckets = BUCKETS;
    // 初始化所有桶为空
    for (int i = 0; i < p->__buckets; i++) {
        p->__heads[i] = NULL;
        p->__tails[i] = NULL;
    }

    p->__count = 0;

    // 绑定函数指针
    p->put = &__HashMap_put;
    p->get = &__HashMap_get;
    p->dump = &__HashMap_dump;

    return p;
}

// ==========================================
// 5. 主函数 (测试代码)
// ==========================================
int main() {
    // 创建哈希表
    struct HashMap* map = HashMap_new();

    printf("--- 开始插入数据 ---\n");
    // 插入第18页示例中的数据
    // 注意：因为哈希算法的结果取决于具体的字符，
    // 所以这里的桶编号可能与PPT略有不同，取决于getBucket的具体计算
    map->put(map, "y", 2);
    map->put(map, "a", 4);
    map->put(map, "z", 1);
    map->put(map, "b", 3);

    // 插入更多数据测试冲突
    map->put(map, "Hello", 42);
    map->put(map, "World", 99);

    // 测试更新数据
    map->put(map, "z", 100); // 把 z 从 1 改成 100

    // 打印整个Map结构
    map->dump(map);

    printf("\n--- 测试查找数据 ---\n");
    printf("Value of 'a': %d\n", map->get(map, "a", -1));
    printf("Value of 'z': %d\n", map->get(map, "z", -1));
    printf("Value of 'missing': %d\n", map->get(map, "missing", -1));

    return 0;
}