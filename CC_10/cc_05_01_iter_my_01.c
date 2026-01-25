// 2026.01.14 Yi 21:31

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKETS_NUMBER 8

struct __node;
struct map;
struct __methods;
struct __iter;

typedef struct __iter {
    struct map* __target_map;
    int __current_index;
    struct __node* __current;
    void (*del)(struct __iter* self);
    struct __node* (*get)(struct __iter* self);
} __iter;

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

void add(struct map* self, char* key_add, int value_add);
void dump(struct map* self);
void size(struct map* self);
int get(map* self, char* key_get, int error_code);
void iter_del(struct __iter* self);
struct __node* iter_get(struct __iter* self);

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
            printf("index: %d, key: %s, value: %d\n", i, curr->key, curr->value);
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
__iter* iter_create(map* target) {
    int i = 0;
    __iter* new_iter = (__iter*)malloc(sizeof(*new_iter));
    new_iter->del = &iter_del;
    new_iter->get = &iter_get;
    new_iter->__target_map = target;
    // aim to head[0]
    new_iter->__current = target->__head[i];
    new_iter->__current_index = i;
    
    
    if (new_iter->__current == NULL) {
        // head is NULL, need to find  a othre one
        for (; i < target->__map_buckets_number; i++) {
            new_iter->__current = target->__head[i];
            if (target->__head[i] != NULL) {
                new_iter->__current_index = i;
                printf("iter's current aimed at map's index [%d]", i);
                return new_iter;
            }
        }
        printf("whole map is empty\n");
        free(new_iter);
        return NULL;
    } else {
        printf("aimed at index[%d]\n",i);
        return  new_iter;
    }
}

void iter_del(struct __iter* self) {
    // TODO
    free(self);
    printf("iter del success\n");
}

struct __node* iter_get(struct __iter* self) {
    // __current must be useable
    __node* temp = self->__current;

    // 链表后移
    self->__current=self->__current->__next;
    // 如果链表空了，找下一个index
    while(self->__current==NULL) {
        // this index is over
        if (self->__current_index < self->__target_map->__map_buckets_number-1) {
            self->__current_index++;
            self->__current=self->__target_map->__head[self->__current_index];
        } else {
            break;
        }
    }
    return temp;
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

    printf("\n--- check iter ---\n");
    __iter* my_iter = iter_create(my_map);
    if (my_iter == NULL) {
        printf("int main() is over.\n");
       return 0; 
    }
    while (my_iter->__current) {
        __node* curr = my_iter->get(my_iter);
        printf("iter result: key: %s, value: %d\n",curr->key,curr->value);
    }

    return 0;
}