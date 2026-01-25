// 2026.01.13 Yi 13:55
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct __map_node {
    char* key;
    int value;
    struct __map_node* __prev;
    struct __map_node* __next;
} __map_node;

typedef struct Map {
    struct __map_node* __head;
    struct __map_node* __tail;
    int count;
    void (*put)(struct Map* self, char* put_key, int put_value);
    int (*get)(struct Map* self, char* get_key);
    void (*size)(struct Map* self);
    void (*dump)(struct Map* self);
    void (*del)(struct Map* self);
} Map;

__map_node* __find(Map* self, char* find_key) {
    __map_node* curr = self->__head;
    while (curr) {
        if (strcmp(find_key, curr->key) == 0) {
            return curr;
        }
        curr=curr->__next;
    }
    return NULL;
}

void __put(struct Map* self, char* put_key, int put_value) {
    // check key exit
    __map_node* find_result = __find(self, put_key);
    if (find_result) {
        // key has exit
        find_result->value = put_value;
        return;
    } else {
        // key has not exit
        __map_node* new_node = (__map_node*)malloc(sizeof(*new_node));
        self->count++;
        if (self->__head == NULL) {
            // whole map is empty
            self->__head = new_node;
        }
        // map is not empty
        new_node->key = strdup(put_key);
        new_node->value = put_value;
        new_node->__prev = self->__tail;
        new_node->__next = NULL;
        if (self->__tail != NULL) {
            // 只有当旧尾巴存在时，才能把它的 next 指向新节点
            self->__tail->__next = new_node;
        }
        self->__tail = new_node;
    }
}

int __get(struct Map* self, char* get_key) {
    __map_node* curr = __find(self, get_key);
    if (curr) {
        // find it !
        return curr->value;
    }
    else {
        // not find it…
        printf("didn't find the key %s in map\n", get_key);
        return -1;
    }
}

void __size(struct Map* self) {
    printf("map's size is %d\n", self->count);
}

void __dump(struct Map* self) {
    __map_node* curr = self->__head;
    while (curr) {
        printf("%s = %d\n", curr->key, curr->value);
        curr=curr->__next;
    }
}

void __del(struct Map* self) {
    __map_node *curr, *next;
    curr = self->__head;
    while (curr) {
        free(curr->key);
        // 为什么这里可以不free int 呢？
        next = curr->__next;
        free(curr);
        curr=next;
    }
    free(self);
}

Map* Map_new() {
    Map* new_Map = (Map*)malloc(sizeof(Map));
    new_Map->__head = NULL;
    new_Map->__tail=NULL;

    new_Map->put = &__put;
    new_Map->get = &__get;
    new_Map->size = &__size; 
    new_Map->dump = &__dump;
    new_Map->del = &__del;
    new_Map->count = 0;
    return new_Map;
}

int main(void) {
    Map* my_map = Map_new();
    my_map->put(my_map,"a",10);
    my_map->put(my_map,"b",20);
    my_map->put(my_map,"c",30);
    my_map->put(my_map,"d",40);
    my_map->put(my_map,"f",50);
    my_map->dump(my_map);
    my_map->put(my_map, "a", 100);
    my_map->dump(my_map);
    
    int try1=my_map->get(my_map,"a");
    printf("%d\n", try1);
    int try2 = my_map->get(my_map, "r");
    printf("%d\n", try2);

    my_map->del(my_map);
    return 0;
}
