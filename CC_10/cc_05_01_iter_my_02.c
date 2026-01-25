// 2026.01.14 Yi 21:31
#include <_stdio.h>
#include <_string.h>
#include <ptrcheck.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKETS_NUMBER 8
/**/
struct __node;
struct map;
struct __methods;
struct iter;
/**/
typedef struct iter {
    struct __node* __current_node;
    int __curr_index;
    struct map* target;
    struct __node* (*iter_next)(struct iter* slef);
} iter;

typedef struct __node {
    char* key;
    int value;
    struct __node* __prev;
    struct __node* __next;
} __node;

typedef struct __methods {
    void (*add)(struct map* self, char* key_add, int value_add);
    void (*dump)(struct map* self);
    void (*size)(struct map* self);
    int (*get)(struct map* self, char* key_get, int error_code);
} __methods;

typedef struct map {
    int __map_buckets_number;
    int __map_count;
    struct __node* __head[BUCKETS_NUMBER];
    struct __node* __tail[BUCKETS_NUMBER];
    struct __methods methods;
} map;
/**/
void add(struct map* self, char* key_add, int value_add);
void dump(struct map* self);
void size(struct map* self);
int get(map* self, char* key_get, int error_code);
__node* iter_next(struct iter* self);

map* map_create() {
    map* new_map = (map*)malloc(sizeof(*new_map));
    new_map->__map_buckets_number = BUCKETS_NUMBER;
    new_map->__map_count = 0;
    new_map->methods.add = &add;
    new_map->methods.dump = &dump;
    new_map->methods.size = &size;
    new_map->methods.get = &get;
    for (int i = 0; i < BUCKETS_NUMBER; i++) {
        new_map->__head[i] = NULL;
        new_map->__tail[i] = NULL;
    }
    printf("new map created success.\n");
    return new_map;
}

int get_hash_index(char* str, int buckets_number) {
    unsigned int index = 123456;
    for (; *str; str++) {
        index = (index << 3) ^ *str;
    }
    return index % buckets_number;
}

__node* check_index(map* self, int check_index, char* check_key) {
    __node* curr = self->__head[check_index];
    while (curr) {
        if (strcmp(curr->key, check_key) == 0) {
            // find the key has exit
            return curr;
        }
        curr = curr->__next;
    }
    return NULL;
}

void add(map* self, char* key_add, int value_add) {
    int add_index = get_hash_index(key_add, self->__map_buckets_number);
    __node* the_node = check_index(self, add_index, key_add);
    if (the_node != NULL) {
        // the_node is not new, only need change value
        the_node->value = value_add;
        return;
    }
    // the_node is new
    // new node has ready to connect
    __node* new_node = (__node*)malloc(sizeof(*new_node));
    new_node->key = strdup(key_add);
    new_node->value = value_add;
    if (self->__head[add_index] == NULL) {
        // node is the first
        self->__head[add_index] = new_node;
        new_node->__prev = NULL;
    } else {
        // node is not the first node of chain
        self->__tail[add_index]->__next = new_node;
        new_node->__prev = self->__tail[add_index];
    }
    self->__map_count++;
    new_node->__next = NULL;
    self->__tail[add_index] = new_node;
}

void size(struct map* self) {
    printf("size is %d\n", self->__map_count);
}

void dump(struct map* self) {
    __node* curr;
    for (int i = 0; i < self->__map_buckets_number; i++) {
        curr = self->__head[i];
        while (curr) {
            printf("hash index is %d, key is %s, value is %d\n", i, curr->key, curr->value);
            curr = curr->__next;
        }
    }
    printf("dump over\n");
}

int get(map* self, char* key_get, int error_code) {

    int hash_index = get_hash_index(key_get, self->__map_buckets_number);
    __node* curr = check_index(self, hash_index, key_get);
    if (curr == NULL) {
        // didn't find the key in node
        // or, node is not exit
        return error_code;
    } else {
        // find the node
        return curr->value;
    }
}
iter* iter_create(map* self) {
    iter* retval_iter = (iter*)malloc(sizeof(*retval_iter));
    retval_iter->__curr_index = 0;
    retval_iter->target = self;
    retval_iter->iter_next = &iter_next;
    retval_iter->__current_node = self->__head[retval_iter->__curr_index];
    // iter_node has point to index[0], may be NULL!
    printf("iter has been made\n");
    return retval_iter;
}

__node* iter_next(struct iter* self) {
    __node* retval_node = self->__current_node;
    // retavl_node may be NULL!

    // check if the retavl_node is NULL
    while (retval_node == NULL) {
        // retval_node is NULL! need to find a new node
        self->__curr_index++;
        if (self->__curr_index >= self->target->__map_buckets_number) {
            return NULL;
            // iter is over
        }
        retval_node = self->target->__head[self->__curr_index];
        
    }
    self->__current_node = retval_node;
    // retval_node is not NULL
    self->__current_node = self->__current_node->__next;
    // next node may be NULL

    return retval_node;
}

int main() {
    map* my_map = map_create();

    printf("--- 开始插入数据 ---\n");
    my_map->methods.add(my_map, "a", 10);
    my_map->methods.add(my_map, "b", 20);
    my_map->methods.add(my_map, "c", 30);
    my_map->methods.add(my_map, "d", 40);
    my_map->methods.add(my_map, "e", 50);
    my_map->methods.add(my_map, "f", 60);
    my_map->methods.add(my_map, "g", 70);
    my_map->methods.add(my_map, "h", 80);
    my_map->methods.add(my_map, "i", 90);
    my_map->methods.add(my_map, "j", 100);
    my_map->methods.add(my_map, "k", 110);
    printf("--- 数据插入完成 ---\n");

    // check size
    my_map->methods.size(my_map);

    // check dump
    my_map->methods.dump(my_map);

    printf("--- 测试数据修改 ---\n");
    my_map->methods.add(my_map, "c", 3000);
    my_map->methods.add(my_map, "d", 4000);

    printf("--- check map edit ---\n");
    // dump again
    my_map->methods.dump(my_map);

    printf("\n--- 测试查找数据 ---\n");
    printf("Value of 'a': %d\n", my_map->methods.get(my_map, "a", -1));
    printf("Value of 'b': %d\n", my_map->methods.get(my_map, "b", -1));
    printf("Value of 'c': %d\n", my_map->methods.get(my_map, "c", -1));
    printf("Value of 'd': %d\n", my_map->methods.get(my_map, "d", -1));
    printf("Value of 'z': %d\n", my_map->methods.get(my_map, "z", -1));
    printf("Value of 'missing': %d\n", my_map->methods.get(my_map, "missing", -1));

    printf("\n--- make map iter ---\n");
    // make a iter of map
    iter* map_iter = iter_create(my_map);

    printf("\n--- check map iter ---\n");
    // try map_iter;
    __node* curr_node;

    while (1) {
        curr_node = map_iter->iter_next(map_iter);
        if (curr_node == NULL) {
            break;
        }
        printf("index: %d, key: %s, value: %d\n", map_iter->__curr_index, curr_node->key,curr_node->value);  
    }

    free(map_iter);

    printf("\n--- 测试 Iterator 遍历 (修正版) ---\n");

    // 1. 创建迭代器
    iter* map_iter_2 = iter_create(my_map);

    // 用于接收 iter_next 返回值的临时指针
    __node* current_node;

    // 2. 驱动迭代器
    //    调用 map_iter->iter_next()。
    //    如果返回不为 NULL，说明拿到了数据，进入循环体。
    //    如果返回 NULL，说明遍历结束，退出循环。
    while ((current_node = map_iter_2->iter_next(map_iter_2)) != NULL) {
        printf("index: %d, key: %s, value: %d\n",
            map_iter_2->__curr_index, // 这里显示的 index 可能会有些滞后，但不影响数据正确性
            current_node->key,
            current_node->value);
    }

    return 0;
}