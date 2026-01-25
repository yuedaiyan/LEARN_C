#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================================================================
   1. 定义与常量 (Definitions)
   ========================================================================= */

// 使用特殊值标记空槽和已删除槽
// 注意：因为我们可能使用 int8_t, int16_t 等，所以这些值必须在所有有符号类型中都有效
#define DKIX_EMPTY (-1)
#define DKIX_DUMMY (-2)

// 初始哈希表大小 (必须是 2 的幂)
#define DICT_MINSIZE 8

// 负载因子 (Python 默认是 2/3)
#define USABLE_FRACTION(n) (((n) << 1) / 3)

// 用于对齐内存的宏 (将指针指针对齐到 8 字节边界)
#define ALIGN_SIZE sizeof(void*)
#define ALIGN(x) (((x) + (ALIGN_SIZE - 1)) & ~(ALIGN_SIZE - 1))

typedef struct {
    size_t hash;
    char* key; // 真实Python里是 PyObject*
    char* value; // 真实Python里是 PyObject*
} PyDictKeyEntry;

/*
   核心数据结构：PyDictKeysObject (简化版)
   这是 Python 字典内存布局的灵魂。它是一个变长结构体。

   内存布局如下：
   [ dk_refcnt, dk_size, ... ]  <-- 结构体头部
   [ int8/16/32/64 indices... ] <-- 紧跟在头部后面的索引数组 (稀疏)
   [ Padding ]                  <-- 对齐补白
   [ Entry 0 ]                  <-- 实际数据数组 (稠密)
   [ Entry 1 ]
   ...
*/
typedef struct {
    int32_t dk_size; // 哈希表总槽位数 (例如 8, 16)
    int32_t dk_usable; // 还能插入多少个元素而不需要扩容
    int32_t dk_nentries; // 当前 entries 数组里用了多少个

    // 这是一个变长数组的起始点。
    // 实际上 indices 数据存储在这个 char 数组开始的内存区域。
    // 我们会根据 dk_size 动态决定这里面存的是 int8 还是 int16。
    char dk_indices[];
} PyDictKeysObject;

typedef struct {
    PyDictKeysObject* keys;
} PyDictObject;

/* =========================================================================
   2. 辅助函数 (Helpers)
   ========================================================================= */

// FNV-1a Hash 算法 (高性能且分布均匀，用于替代 Python 的 SipHash)
static inline size_t get_hash(const char* key) {
    size_t hash = 14695981039346656037UL;
    for (const char* p = key; *p; p++) {
        hash ^= (size_t)(unsigned char)(*p);
        hash *= 1099511628211UL;
    }
    return hash;
}

// 获取存储索引所需的字节数 (1, 2, 4, or 8)
static inline int get_index_item_size(int32_t size) {
    if (size <= 0xff)
        return 1;
    if (size <= 0xffff)
        return 2;
    return 4; // 这里简化，不演示 int64 了
}

// 【关键优化】读取索引 (多态)
// 编译器会内联此函数，根据 item_size 生成极高效的汇编
static inline int32_t dk_get_index(PyDictKeysObject* keys, int32_t i) {
    int item_size = get_index_item_size(keys->dk_size);
    if (item_size == 1) {
        return ((int8_t*)(keys->dk_indices))[i];
    } else if (item_size == 2) {
        return ((int16_t*)(keys->dk_indices))[i];
    } else {
        return ((int32_t*)(keys->dk_indices))[i];
    }
}

// 【关键优化】写入索引 (多态)
static inline void dk_set_index(PyDictKeysObject* keys, int32_t i, int32_t val) {
    int item_size = get_index_item_size(keys->dk_size);
    if (item_size == 1) {
        ((int8_t*)(keys->dk_indices))[i] = (int8_t)val;
    } else if (item_size == 2) {
        ((int16_t*)(keys->dk_indices))[i] = (int16_t)val;
    } else {
        ((int32_t*)(keys->dk_indices))[i] = (int32_t)val;
    }
}

// 计算 entries 数组在内存中的起始位置
// 它位于 indices 数组之后，并且经过对齐
static inline PyDictKeyEntry* dk_entries(PyDictKeysObject* keys) {
    int item_size = get_index_item_size(keys->dk_size);
    size_t offset = keys->dk_size * item_size;
    // 偏移量 + 对齐
    offset = ALIGN(offset);
    return (PyDictKeyEntry*)((char*)keys->dk_indices + offset);
}

/* =========================================================================
   3. 核心逻辑 (Core Logic)
   ========================================================================= */

// 创建新的 Keys 对象 (分配一块连续的大内存)
PyDictKeysObject* new_keys_object(int32_t size) {
    if (size < DICT_MINSIZE)
        size = DICT_MINSIZE;

    int item_size = get_index_item_size(size);

    // 1. 计算 Indices 区域大小
    size_t indices_bytes = size * item_size;

    // 2. 计算 Entries 区域大小 (Entry 数量通常是哈希表大小的 2/3)
    // 但为了简化，我们分配 size 个 entry 空间，尽管只会用到 2/3
    size_t entries_bytes = size * sizeof(PyDictKeyEntry);

    // 3. 计算总大小：Header + Indices + Padding + Entries
    size_t total_size = sizeof(PyDictKeysObject) + ALIGN(indices_bytes) + entries_bytes;

    PyDictKeysObject* keys = (PyDictKeysObject*)malloc(total_size);

    keys->dk_size = size;
    keys->dk_usable = USABLE_FRACTION(size);
    keys->dk_nentries = 0;

    // 初始化 indices 为 DKIX_EMPTY (-1)
    // 这是一个内存操作优化：如果全是 1 字节，直接 memset
    if (item_size == 1) {
        memset(keys->dk_indices, 0xff, indices_bytes); // 0xff is -1 in int8
    } else {
        // 对于 int16/int32，必须手动循环赋值
        for (int i = 0; i < size; i++) {
            dk_set_index(keys, i, DKIX_EMPTY);
        }
    }

    return keys;
}

// 【核心算法】查找插入位置 (Lookup)
// 返回值：
//   如果是正数：找到了 key，返回该 key 在 entries 数组中的下标
//   如果是 DKIX_EMPTY：没找到，返回应该插入的 indices 槽位下标 (注意是负数的补码形式或其他方式返回，这里为了简单，通过传出参数返回)
//
// 参数：
//   found_idx: 如果找到了 key，这里存储 entries 下标
//   return: indices 数组的下标 (也就是哈希槽的位置)
int32_t find_index(PyDictKeysObject* keys, const char* key, size_t hash, int32_t* found_entry_idx) {
    int32_t mask = keys->dk_size - 1;
    int32_t i = (int32_t)hash & mask;
    int32_t ix = dk_get_index(keys, i);

    // 1. 如果槽位为空，直接返回
    if (ix == DKIX_EMPTY) {
        *found_entry_idx = DKIX_EMPTY;
        return i;
    }

    // 2. 槽位不为空，开始探测
    PyDictKeyEntry* entries = dk_entries(keys);
    size_t perturb = hash;

    while (1) {
        // 检查当前槽位指向的 Entry 是否匹配
        if (ix >= 0) {
            PyDictKeyEntry* ep = &entries[ix];
            // 必须检查 Hash 是否一致（快速过滤），再检查字符串
            if (ep->hash == hash && strcmp(ep->key, key) == 0) {
                *found_entry_idx = ix; // 找到了！
                return i;
            }
        }

        // 3. Python 的扰动探测算法 (Perturbation Shift)
        // 这里的逻辑是防止某些特定的哈希模式导致无限循环或聚集
        perturb >>= 5;
        i = (i * 5 + perturb + 1) & mask;

        ix = dk_get_index(keys, i);

        if (ix == DKIX_EMPTY) {
            *found_entry_idx = DKIX_EMPTY;
            return i;
        }
    }
}

// 字典扩容 (Insertion Order Preservation happens here!)
// 这也是最消耗性能的地方，但 CPython 做了极致优化
void dict_resize(PyDictObject* mp) {
    PyDictKeysObject* old_keys = mp->keys;
    int32_t new_size = old_keys->dk_size * 2; // 翻倍

    // 分配新的内存块
    PyDictKeysObject* new_keys = new_keys_object(new_size);

    // 获取旧的和新的 entries 数组指针
    PyDictKeyEntry* old_entries = dk_entries(old_keys);
    PyDictKeyEntry* new_entries = dk_entries(new_keys);

    // --- 核心点：重建索引，保留顺序 ---
    // 我们不需要重新 hash (用缓存的 hash)，也不需要重新插入数据
    // 我们直接按顺序拷贝 entry，并在新的 sparse indices 里占位

    for (int i = 0; i < old_keys->dk_nentries; i++) {
        PyDictKeyEntry* ep = &old_entries[i];

        if (ep->value != NULL) { // 只有非删除的元素才迁移
            // 1. 在新数组中追加 Entry (紧密排列)
            int32_t new_entry_idx = new_keys->dk_nentries++;
            new_entries[new_entry_idx] = *ep; // 结构体直接拷贝

            // 2. 重新计算在新 indices 中的位置 (Re-probing)
            int32_t mask = new_size - 1;
            int32_t idx = (int32_t)ep->hash & mask;
            size_t perturb = ep->hash;

            // 这是一个简化版的探测，因为我们知道肯定没重复，且肯定有空位
            while (dk_get_index(new_keys, idx) != DKIX_EMPTY) {
                perturb >>= 5;
                idx = (idx * 5 + perturb + 1) & mask;
            }

            // 3. 写入新的稀疏索引
            dk_set_index(new_keys, idx, new_entry_idx);
        }
    }

    // 释放旧内存，挂载新内存
    free(old_keys);
    mp->keys = new_keys;
}

// 插入数据
void pydict_put(PyDictObject* d, const char* key, const char* val) {
    // 1. 检查扩容
    if (d->keys->dk_usable <= 0) {
        dict_resize(d);
    }

    size_t hash = get_hash(key);
    int32_t found_entry_idx;
    // 2. 查找位置
    int32_t idx_in_indices = find_index(d->keys, key, hash, &found_entry_idx);

    // 3. 如果 Key 已存在 -> 更新
    if (found_entry_idx != DKIX_EMPTY) {
        PyDictKeyEntry* entries = dk_entries(d->keys);
        entries[found_entry_idx].value = (char*)val; // 真实场景需要处理引用计数
        return;
    }

    // 4. 如果 Key 不存在 -> 插入
    // 4.1 在稠密数组 entries 的末尾追加
    int32_t new_entry_pos = d->keys->dk_nentries;
    PyDictKeyEntry* entries = dk_entries(d->keys);

    entries[new_entry_pos].hash = hash;
    entries[new_entry_pos].key = strdup(key); // 模拟 owning memory
    entries[new_entry_pos].value = strdup(val);

    // 4.2 在稀疏数组 indices 中记录下标
    dk_set_index(d->keys, idx_in_indices, new_entry_pos);

    // 4.3 更新计数器
    d->keys->dk_nentries++;
    d->keys->dk_usable--;
}

// 获取数据
char* pydict_get(PyDictObject* d, const char* key) {
    size_t hash = get_hash(key);
    int32_t found_entry_idx;
    find_index(d->keys, key, hash, &found_entry_idx);

    if (found_entry_idx != DKIX_EMPTY) {
        PyDictKeyEntry* entries = dk_entries(d->keys);
        return entries[found_entry_idx].value;
    }
    return NULL;
}

/* =========================================================================
   4. 调试与主程序
   ========================================================================= */

void print_debug(PyDictObject* d) {
    PyDictKeysObject* k = d->keys;
    printf("\n--- Dict State (Size: %d, Entries: %d) ---\n", k->dk_size, k->dk_nentries);

    // 打印 indices (稀疏部分)
    printf("Indices Type: int%d_t\n", get_index_item_size(k->dk_size) * 8);
    printf("Indices: [ ");
    for (int i = 0; i < k->dk_size; i++) {
        int32_t val = dk_get_index(k, i);
        if (val == DKIX_EMPTY)
            printf("_ ");
        else
            printf("%d ", val);
    }
    printf("]\n");

    // 打印 entries (稠密部分)
    PyDictKeyEntry* entries = dk_entries(k);
    printf("Entries: \n");
    for (int i = 0; i < k->dk_nentries; i++) {
        printf("  [%d] hash=%zx key=%s val=%s\n", i, entries[i].hash, entries[i].key, entries[i].value);
    }
}

int main() {
    // 初始化字典
    PyDictObject* d = malloc(sizeof(PyDictObject));
    d->keys = new_keys_object(8); // 初始大小 8

    printf("Inserting a, b, c...\n");
    pydict_put(d, "a", "1");
    pydict_put(d, "b", "2");
    pydict_put(d, "c", "3");

    print_debug(d);

    printf("\nGetting 'b': %s\n", pydict_get(d, "b"));

    printf("\nForce Resize (Inserting d, e, f to trigger 2/3 limit)...\n");
    // 8 * 2/3 = 5. 目前有3个，再加2个不扩容，第6个会扩容
    pydict_put(d, "d", "4");
    pydict_put(d, "e", "5");
    pydict_put(d, "f", "6"); // Trigger resize to 16

    print_debug(d);

    return 0;
}