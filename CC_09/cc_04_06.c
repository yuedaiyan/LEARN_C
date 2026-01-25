#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================================================
 * 前置声明 (Forward Declarations)
 * ========================================================= */
struct Map;
struct MapEntry;
struct MapIter;

/* =========================================================
 * 1. 数据结构定义 (对应 PPT Page 3, 4, 7)
 * ========================================================= */

/* 存储键值对的节点 (链表节点) */
struct MapEntry {
    char* key; /* public: 键 */
    int value; /* public: 值 */
    struct MapEntry* __prev; /* private: 链表前驱 */
    struct MapEntry* __next; /* private: 链表后继 */
};

/* 迭代器结构体 (对应 PPT Page 7) */
struct MapIter {
    struct MapEntry* __current; /* private: 当前游标 */

    /* 方法指针 */
    struct MapEntry* (*next)(struct MapIter* self);
    void (*del)(struct MapIter* self);
};

/* Map 主类结构体 (对应 PPT Page 4) */
struct Map {
    /* Attributes (被视为 private) */
    struct MapEntry* __head;
    struct MapEntry* __tail;
    int __count;

    /* Methods (模拟面向对象的方法) */
    void (*put)(struct Map* self, char* key, int value);
    int (*get)(struct Map* self, char* key, int def);
    struct MapIter* (*iter)(struct Map* self);
    void (*dump)(struct Map* self);
    void (*del)(struct Map* self);
};

/* =========================================================
 * 2. 迭代器实现 (PPT 的核心部分 Page 8 - 14)
 * ========================================================= */

/* 对应 PPT Page 9-14: next() 方法的具体实现 */
/* 逻辑：先保存当前节点，游标后移，然后返回保存的节点 */
struct MapEntry* __MapIter_next(struct MapIter* self) {
    struct MapEntry* retval = self->__current;
    if (retval == NULL)
        return NULL;
    self->__current = self->__current->__next;
    return retval;
}

/* 销毁迭代器 */
void __MapIter_del(struct MapIter* self) {
    free(self);
}

/* 对应 PPT Page 8: 创建迭代器的构造函数 */
struct MapIter* __Map_iter(struct Map* self) {
    struct MapIter* iter = malloc(sizeof(*iter));
    iter->__current = self->__head; /* 游标初始化指向链表头 */
    iter->next = &__MapIter_next; /* 绑定方法 */
    iter->del = &__MapIter_del; /* 绑定方法 */
    return iter;
}

/* =========================================================
 * 3. Map 的基础逻辑实现 (链表操作)
 * ========================================================= */

/* 辅助函数：创建新节点 */
struct MapEntry* __MapEntry_new(char* key, int value) {
    struct MapEntry* e = malloc(sizeof(*e));
    e->key = strdup(key); // 复制字符串，防止外部修改
    e->value = value;
    e->__prev = NULL;
    e->__next = NULL;
    return e;
}

/* 放数据：如果存在则更新，不存在则追加到链表尾部 */
void __Map_put(struct Map* self, char* key, int value) {
    struct MapEntry* cur = self->__head;
    // 1. 查找是否存在
    while (cur != NULL) {
        if (strcmp(cur->key, key) == 0) {
            cur->value = value; // 更新值
            return;
        }
        cur = cur->__next;
    }

    // 2. 不存在，创建新节点
    struct MapEntry* new_entry = __MapEntry_new(key, value);
    if (self->__head == NULL) {
        self->__head = new_entry;
        self->__tail = new_entry;
    } else {
        self->__tail->__next = new_entry;
        new_entry->__prev = self->__tail;
        self->__tail = new_entry;
    }
    self->__count++;
}

/* 取数据：如果没找到返回默认值 def */
int __Map_get(struct Map* self, char* key, int def) {
    struct MapEntry* cur = self->__head;
    while (cur != NULL) {
        if (strcmp(cur->key, key) == 0) {
            return cur->value;
        }
        cur = cur->__next;
    }
    return def;
}

/* 打印整个 Map (调试用) */
void __Map_dump(struct Map* self) {
    struct MapEntry* cur = self->__head;
    printf("Object Map count=%d\n", self->__count);
    while (cur != NULL) {
        printf(" %s=%d\n", cur->key, cur->value);
        cur = cur->__next;
    }
}

/* 销毁 Map 及其所有节点 */
void __Map_del(struct Map* self) {
    struct MapEntry* cur = self->__head;
    while (cur != NULL) {
        struct MapEntry* next = cur->__next;
        free(cur->key); // 释放 strdup 分配的内存
        free(cur);
        cur = next;
    }
    free(self);
}

/* Map 的构造函数 */
struct Map* Map_new() {
    struct Map* map = malloc(sizeof(*map));
    map->__head = NULL;
    map->__tail = NULL;
    map->__count = 0;

    // 绑定函数指针
    map->put = &__Map_put;
    map->get = &__Map_get;
    map->iter = &__Map_iter; // 绑定迭代器工厂方法
    map->dump = &__Map_dump;
    map->del = &__Map_del;

    return map;
}

/* =========================================================
 * 4. 主函数 (对应 PPT Page 2)
 * ========================================================= */
int main(void) {
    // 创建 Map 对象
    struct Map* map = Map_new();
    struct MapEntry* cur;
    struct MapIter* iter;

    printf("Testing Map class\n"); // 修改了一点输出以匹配 Page 2 右侧截图

    // 插入数据
    map->put(map, "z", 8);
    map->put(map, "z", 1); // 更新 z 的值
    map->put(map, "y", 2);
    map->put(map, "b", 3);
    map->put(map, "a", 4);

    // 打印当前状态
    map->dump(map);

    // 测试 get
    printf("z=%d\n", map->get(map, "z", 42));
    printf("x=%d\n", map->get(map, "x", 42)); // x 不存在，应返回 42

    printf("\nIterate\n");

    // 核心：使用迭代器
    iter = map->iter(map); // 获取迭代器
    while (1) {
        cur = iter->next(iter); // 获取下一个节点
        if (cur == NULL)
            break; // 遍历结束
        printf("%s=%d\n", cur->key, cur->value);
    }

    // 清理内存
    iter->del(iter);
    map->del(map);

    return 0;
}