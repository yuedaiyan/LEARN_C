#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct dnode {
    char* key;
    char* value;
};

struct pldict {
    int alloc; // 总容量
    int length; // 当前元素数量
    struct dnode* items; // 指向节点数组的指针
};


// 哈希函数
unsigned long hash_string(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
} // 传入：字符串
  // 返回：字符串 hash 值


// 构造函数：初始化字典
struct pldict* pldict_new() {
    struct pldict* p = malloc(sizeof(*p));
    p->length = 0;
    p->alloc = 2; // 初始容量

    // 分配内存并初始化为 NULL
    p->items = malloc(p->alloc * sizeof(struct dnode));
    for (int i = 0; i < p->alloc; i++) {
        p->items[i].key = NULL;
        p->items[i].value = NULL;
    }
    return p;
} // 返回一个 0-2 的字典


// 核心查找函数 (Find)
struct dnode* pldict_find(struct pldict* self, char* key) {
    unsigned long h = hash_string(key);
    int i = h % self->alloc;

    // 循环查找
    while (1) {
        // 1. 如果位置为空，或者 key 匹配，直接返回该节点指针
        // 注意：这里能退出的前提是表永远不为 100% 满
        if (self->items[i].key == NULL || strcmp(self->items[i].key, key) == 0) {
            return &self->items[i];
        }

        // 2. 发生冲突，向前移动
        i = i - 1;

        // 3. 处理循环边界 (Wrap around)
        if (i < 0) {
            i = i + self->alloc;
        }
    }
} // 传入：查找目标字符串
  // 返回：匹配 key 的节点或第一个 NULL 节点


// 插入函数 (Put)
void pldict_put(struct pldict* self, char* key, char* value) {

    // 子函数：扩容检查
    if (self->length >= self->alloc * 0.66 || self->length >= self->alloc - 1) {
        printf("--- Debug: Resizing for key '%s' (Alloc: %d -> %d) ---\n", key, self->alloc, self->alloc * 2);

        int old_alloc = self->alloc;
        struct dnode* old_items = self->items; // 保存旧数组指针

        // 更新字典属性为新数组
        self->alloc = self->alloc * 2;
        self->items = malloc(self->alloc * sizeof(struct dnode));

        // 初始化新数组
        for (int i = 0; i < self->alloc; i++) {
            self->items[i].key = NULL;
            self->items[i].value = NULL;
        }

        // Re-hash: 将旧数据搬运到新数组
        // 注意：这里不能重置 self->length，因为我们只是在搬运
        for (int i = 0; i < old_alloc; i++) {
            if (old_items[i].key == NULL)
                continue;

            // 在新数组中寻找位置 (此时 self->items 已经是新的了)
            struct dnode* new_item = pldict_find(self, old_items[i].key);

            // 直接转移指针，不需要重新 malloc
            new_item->key = old_items[i].key;
            new_item->value = old_items[i].value;
        }

        free(old_items); // 释放旧数组的“骨架”，但保留字符串内容（因为已经转移了）
    } // 如果条件为真（需要扩容） → 执行函数（扩容）

    // 2. 查找插入位置
    struct dnode* node = pldict_find(self, key);
    // 获得相同 key 节点或 NULL 节点

    // 3. 如果是更新现有 Key
    if (node->key != NULL) {
        free(node->value); // 释放旧值
        node->value = malloc(strlen(value) + 1);
        strcpy(node->value, value);
        return;
    } // 如果key=NULL  → 添加key+val
    // 4. 如果是新插入 Key
    node->value = malloc(strlen(value) + 1);
    strcpy(node->value, value);

    node->key = malloc(strlen(key) + 1);
    strcpy(node->key, key);

    self->length++;
}


//辅助函数：获取值
char* pldict_get(struct pldict* self, char* key) {
    struct dnode* node = pldict_find(self, key);
    // find函数 → 返回 NULL ｜｜ key 节点
    if (node->key != NULL) {
        return node->value;
    }
    return NULL;
} 
// 如果key存在，返回对应的val
// 如果key不存在，返回NULL


// 辅助函数：打印字典内部状态
void print_debug(struct pldict* d) {
    printf("Dict State (Length: %d, Alloc: %d):\n", d->length, d->alloc);
    // 打印当前节点“长度”和“实际长度”
    for (int i = 0; i < d->alloc; i++) {
        printf("  [%d]: ", i);
        if (d->items[i].key) {
            printf("Key='%s', Value='%s' (Hash%%Alloc = %lu)\n",
                d->items[i].key,
                d->items[i].value,
                hash_string(d->items[i].key) % d->alloc);
        } else {
            printf("(empty)\n");
        }
    } // 逐个节点打印 → NULL 或 节点内容（key+val）
    printf("---------------------------\n");
}


int main() {
    struct pldict* d = pldict_new();

    // 1. 插入数据
    printf("Inserting 'z'...\n");
    pldict_put(d, "z", "Catch phrase");

    // 注意：由于初始容量是2，插入第2个元素时如果不扩容，表就满了(100%)。
    // 为了安全，上面的代码修改为更积极的扩容，所以这里插入 'y' 时可能就会触发扩容。
    printf("Inserting 'y'...\n");
    pldict_put(d, "y", "B");

    print_debug(d);

    // 2. 插入更多数据
    printf("Inserting 'c'...\n");
    pldict_put(d, "c", "42");

    print_debug(d);

    pldict_put(d, "a", "Apple");
    pldict_put(d, "b", "Banana");

    print_debug(d);

    // 4. 测试读取
    printf("Value for 'z': %s\n", pldict_get(d, "z"));
    char* val_c = pldict_get(d, "c");
    printf("Value for 'c': %s\n", val_c ? val_c : "NULL");

    // 测试读取不存在的键 (如果扩容逻辑不对，这里会死循环)
    printf("Value for 'missing': %s\n", pldict_get(d, "missing_key") ? "Found" : "NULL");

    return 0;
}