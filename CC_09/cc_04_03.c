#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==========================================
 * 1. 结构体定义 (对应 PPT 第 3, 4 页)
 * ========================================== */

/* 前向声明 */
struct Map;

/* 链表节点结构 (MapEntry) */
struct MapEntry {
    char* key; /* 公有: 键 */
    int value; /* 公有: 值 */
    struct MapEntry* __prev; /* 私有: 前驱指针 */
    struct MapEntry* __next; /* 私有: 后继指针 */
};

/* Map 容器结构 (Map) */
struct Map {
    /* 私有属性 (Private attributes) */
    struct MapEntry* __head;
    struct MapEntry* __tail;
    int __count;

    /* 公有方法 (Public methods) - 函数指针 */
    void (*put)(struct Map* self, char* key, int value);
    int (*get)(struct Map* self, char* key, int def);
    int (*size)(struct Map* self);
    void (*dump)(struct Map* self);
    void (*del)(struct Map* self);
};

/* ==========================================
 * 2. 函数声明 (对应 PPT 各页的方法定义)
 * ========================================== */
void __Map_put(struct Map* self, char* key, int value);
int __Map_get(struct Map* self, char* key, int def);
int __Map_size(struct Map* self);
void __Map_dump(struct Map* self);
void __Map_del(struct Map* self);
struct Map* Map_new();

/* 辅助函数：查找节点 (PPT中提到了但没写具体代码) */
struct MapEntry* __Map_find(struct Map* self, char* key) {
    struct MapEntry* cur = self->__head;
    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            return cur;
        }
        cur = cur->__next;
    }
    return NULL;
}

/* ==========================================
 * 3. 方法的具体实现
 * ========================================== */

/* 构造函数 (Constructor) - PPT 第 5 页 */
struct Map* Map_new() {
    struct Map* p = (struct Map*)malloc(sizeof(struct Map));

    // 初始化属性
    p->__head = NULL;
    p->__tail = NULL;
    p->__count = 0;

    // 绑定方法 (这是封装的关键)
    p->put = &__Map_put;
    p->get = &__Map_get;
    p->size = &__Map_size;
    p->dump = &__Map_dump;
    p->del = &__Map_del;

    return p;
}

/* Map_put - PPT 第 9 页 (核心逻辑补全) */
void __Map_put(struct Map* self, char* key, int value) {
    // 1. 先检查 key 是否已经存在
    struct MapEntry* old = __Map_find(self, key);
    if (old) {
        old->value = value; // 如果存在，直接更新值
        return;
    }

    // 2. 如果不存在，创建新节点
    struct MapEntry* new_node = (struct MapEntry*)malloc(sizeof(struct MapEntry));
    // 注意：PPT提到 key 保存在 newly allocated space，所以用 strdup 复制字符串
    new_node->key = strdup(key);
    new_node->value = value;
    new_node->__next = NULL;
    new_node->__prev = NULL;

    // 3. 将新节点加入双向链表末尾
    if (self->__head == NULL) {
        // 链表为空
        self->__head = new_node;
        self->__tail = new_node;
    } else {
        // 链表不为空，接到 tail 后面
        new_node->__prev = self->__tail;
        self->__tail->__next = new_node;
        self->__tail = new_node;
    }

    self->__count++;
}

/* Map_get - PPT 第 8 页 */
int __Map_get(struct Map* self, char* key, int def) {
    struct MapEntry* retval = __Map_find(self, key);
    if (retval == NULL)
        return def;
    return retval->value;
}

/* Map_dump - PPT 第 6 页 */
void __Map_dump(struct Map* self) {
    struct MapEntry* cur;
    printf("Object Map count=%d\n", self->__count);
    for (cur = self->__head; cur != NULL; cur = cur->__next) {
        printf(" %s=%d\n", cur->key, cur->value);
    }
}

/* Map_del (析构函数) - PPT 第 7 页 */
void __Map_del(struct Map* self) {
    struct MapEntry *cur, *next;
    cur = self->__head;
    while (cur) {
        free(cur->key); // 释放 strdup 分配的字符串内存
        next = cur->__next; // 先保存下一个，因为马上要 free(cur)
        free(cur); // 释放节点内存
        cur = next;
    }
    free(self); // 最后释放 Map 结构体本身
}

/* Map_size (PPT里没写，简单实现一下) */
int __Map_size(struct Map* self) {
    return self->__count;
}

/* ==========================================
 * 4. 主函数 (Main) - 对应 PPT 第 2 页
 * ========================================== */
int main(void) {
    struct Map* map = Map_new();

    printf("Testing Map class\n");

    // 使用面向对象风格的调用 map->put(...)
    map->put(map, "z", 8);
    map->put(map, "z", 1); // 更新 z 的值
    map->put(map, "y", 2);
    map->put(map, "b", 3);
    map->put(map, "a", 4);

    map->dump(map);

    printf("z=%d\n", map->get(map, "z", 42));
    printf("x=%d\n", map->get(map, "x", 42)); // x 不存在，应返回 42

    map->del(map); // 清理内存

    return 0;
}