#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 标记哈希槽为空
#define DKIX_EMPTY -1

// 简单的字符串哈希函数 (djb2算法)
size_t get_hash(const char* str) {
    size_t hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

// ---------------------------------------------------------
// 核心数据结构
// ---------------------------------------------------------

// 1. 存储实际数据的单元 (Entry)
// 对应 Python 中的 PyDictKeyEntry
typedef struct {
    size_t hash; // 缓存哈希值，避免扩容时重算
    char* key;
    char* value;
} Entry;

// 2. 字典对象
// 对应 Python 中的 PyDictObject
typedef struct {
    // 【稀疏数组】indices
    // 在真实Python中，这里是 void*，根据大小指向 int8/int16/int32 数组
    // 这里为了代码易读，我们直接用 int
    int* indices;
    size_t indices_capacity; // 稀疏数组的大小 (例如 8)

    // 【稠密数组】entries
    Entry* entries;
    size_t entries_capacity; // entries 数组分配的内存大小
    size_t n_entries; // 当前实际存储的元素个数 (usability)
} PyDict;

// ---------------------------------------------------------
// 字典操作实现
// ---------------------------------------------------------

// 初始化字典
PyDict* pydict_create() {
    PyDict* d = (PyDict*)malloc(sizeof(PyDict));

    // 初始容量设为 8
    d->indices_capacity = 8;
    d->indices = (int*)malloc(d->indices_capacity * sizeof(int));
    // 将稀疏数组初始化为 -1 (EMPTY)
    for (int i = 0; i < d->indices_capacity; i++)
        d->indices[i] = DKIX_EMPTY;

    d->entries_capacity = 8; // 通常 entries 大小约为 indices 的 2/3
    d->entries = (Entry*)malloc(d->entries_capacity * sizeof(Entry));
    d->n_entries = 0;

    return d;
}

// 扩容函数 (核心逻辑！)
// 当哈希表拥挤时，只扩大 indices，并重建索引。
// entries 数组只有在满了的时候才 realloc，但数据不需要移动！
void pydict_resize(PyDict* d) {
    size_t old_cap = d->indices_capacity;
    size_t new_cap = old_cap * 2; // 简单的翻倍

    printf("\n[Resize] Triggered! Expanding indices from %zu to %zu\n", old_cap, new_cap);

    // 1. 分配新的、更大的稀疏数组
    int* new_indices = (int*)malloc(new_cap * sizeof(int));
    for (int i = 0; i < new_cap; i++)
        new_indices[i] = DKIX_EMPTY;

    // 2. 重建索引 (Re-indexing)
    // 遍历现有的 entries，把它们的下标重新填入新的 indices 数组
    // 注意：我们不需要移动 entries 里的任何数据！这是最快的地方！
    for (int i = 0; i < d->n_entries; i++) {
        Entry* entry = &d->entries[i];
        size_t hash = entry->hash;
        size_t idx = hash % new_cap;

        // 简单的线性探测处理碰撞
        while (new_indices[idx] != DKIX_EMPTY) {
            idx = (idx + 1) % new_cap;
        }
        new_indices[idx] = i; // 将 Entry 的下标填入哈希桶
    }

    // 3. 释放旧的 indices，换上新的
    free(d->indices);
    d->indices = new_indices;
    d->indices_capacity = new_cap;

    // (如果 entries 数组满了，这里也应该 realloc entries，代码略简略)
    if (d->n_entries >= d->entries_capacity) {
        d->entries_capacity *= 2;
        d->entries = realloc(d->entries, d->entries_capacity * sizeof(Entry));
    }
}

// 插入数据
void pydict_set(PyDict* d, const char* key, const char* value) {
    // 检查负载因子，如果太满就扩容 (例如超过 2/3)
    if (d->n_entries * 3 >= d->indices_capacity * 2) {
        pydict_resize(d);
    }

    size_t hash = get_hash(key);
    size_t idx = hash % d->indices_capacity;

    // 1. 在 indices 中查找位置 (处理碰撞)
    // 真实的 Python 会先查找 key 是否已经存在以进行覆盖，
    // 这里为了演示“插入”，假设 key 都不重复。
    while (d->indices[idx] != DKIX_EMPTY) {
        // 如果这里要做覆盖逻辑：
        // int existing_entry_idx = d->indices[idx];
        // if (strcmp(d->entries[existing_entry_idx].key, key) == 0) { 覆盖... return; }

        // 发生碰撞！线性探测下一个位置
        printf("  [Collision] Hash collision for key '%s' at index %zu, probing...\n", key, idx);
        idx = (idx + 1) % d->indices_capacity;
    }

    // 2. 将数据追加到 entries (稠密数组) 的末尾
    // 这一步保证了插入顺序！
    int entry_idx = d->n_entries;
    d->entries[entry_idx].hash = hash;
    d->entries[entry_idx].key = strdup(key); // 复制字符串
    d->entries[entry_idx].value = strdup(value);
    d->n_entries++;

    // 3. 将 entries 的下标填入 indices (稀疏数组)
    d->indices[idx] = entry_idx;
}

// 获取数据
char* pydict_get(PyDict* d, const char* key) {
    size_t hash = get_hash(key);
    size_t idx = hash % d->indices_capacity;

    // 在 indices 数组中探测
    while (d->indices[idx] != DKIX_EMPTY) {
        int entry_idx = d->indices[idx]; // 拿到 entries 的下标
        Entry* e = &d->entries[entry_idx];

        // 检查：哈希值匹配 且 键字符串匹配
        if (e->hash == hash && strcmp(e->key, key) == 0) {
            return e->value; // 找到了
        }

        // 没找到，继续探测
        idx = (idx + 1) % d->indices_capacity;
    }
    return NULL; // 没找到
}

// ---------------------------------------------------------
// 调试与演示工具
// ---------------------------------------------------------

void print_debug_view(PyDict* d) {
    printf("\n=== Internal Memory Layout ===\n");

    printf("1. Indices (Sparse Hash Table, Size: %zu):\n   [", d->indices_capacity);
    for (int i = 0; i < d->indices_capacity; i++) {
        if (d->indices[i] == DKIX_EMPTY)
            printf(" -1");
        else
            printf(" %2d", d->indices[i]);
    }
    printf(" ]\n");

    printf("2. Entries (Dense Data Array, Insertion Order):\n");
    for (int i = 0; i < d->n_entries; i++) {
        printf("   [%d] Hash:%lu | Key:\"%s\" -> Val:\"%s\"\n",
            i, d->entries[i].hash, d->entries[i].key, d->entries[i].value);
    }
    printf("==============================\n");
}

int main() {
    PyDict* d = pydict_create();

    printf("--- Inserting Data ---\n");
    // 故意构造一些数据，希望能产生碰撞 (取决于 hash 算法)
    // 假设 "A", "I", "Q" 在 mod 8 时可能碰撞
    pydict_set(d, "apple", "red");
    pydict_set(d, "banana", "yellow");
    pydict_set(d, "grape", "purple"); // 假设这里产生碰撞

    print_debug_view(d);

    printf("\n--- Test Lookup ---\n");
    printf("Get 'banana': %s\n", pydict_get(d, "banana"));
    printf("Get 'grape': %s\n", pydict_get(d, "grape"));

    printf("\n--- Test Iteration (Order Preserved?) ---\n");
    for (int i = 0; i < d->n_entries; i++) {
        printf("%d: %s\n", i, d->entries[i].key);
    }

    // 触发扩容测试
    printf("\n--- Triggering Resize ---\n");
    pydict_set(d, "date", "brown");
    pydict_set(d, "eggplant", "purple");
    pydict_set(d, "fig", "green"); // 这应该会触发 resize

    print_debug_view(d);

    return 0;
}