// 2026.01.14 Yi Noon
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node;
struct map;
struct iter;
struct methods;

typedef struct iter {
    struct node* __current;
    void(*iter_next)(struct iter*self);
    void(*iter_del)(struct iter*self);
    } iter;

typedef struct __methods {
    void (*del)(struct map* self);
    void (*add)(struct map* self, char* add_key, int add_value);
    void (*dump)(struct map* sefl);
    void (*size)(struct map* self);
    struct iter* (*creat_iter)(struct map* self);
    struct iter* (*iter)(struct map* self);
} __methods;

typedef struct node {
    char* __key;
    int __value;
    struct node* __prev;
    struct node* __next;
} node;

typedef struct map {
    struct node* __head;
    struct node* __tail;
    int __count;
    struct __methods* __methods;
} map;


void iter_del(struct iter* self) {
    free(self);
    printf("iter del over\n");
}

void iter_next(struct iter* self) {
    while (self->__current) {
        printf("key: %s value: %d\n", self->__current->__key, self->__current->__value);
        self->__current=self->__current->__next;
    }
    printf("iter over\n");
}

struct iter* creat_iter(map* self) {
    iter* new_iter = (iter*)malloc(sizeof(*new_iter));
    new_iter->iter_del = &iter_del;
    new_iter->iter_next = &iter_next;
    new_iter->__current=self->__head;
    return new_iter;
}

void del(struct map* self) {
    node *curr, *next;
    curr = self->__head;
    while (curr) {
        free(curr->__key);
        next = curr->__next;
        free(curr);
        curr=next;
    }
    free(self);
    free(self->__methods);
    printf("del over\n");
}

void dump(struct map* self) {
    node* curr = self->__head;
    while (curr) {
        printf("key: %s value: %d\n",curr->__key,curr->__value);
        curr=curr->__next;
        }
    }

void size(struct map* self) {
    printf("size = %d\n",self->__count);
}

void add(struct map* self, char* add_key, int add_value) {
    node* curr = (node*)malloc(sizeof(*curr));
    curr->__key = strdup(add_key);
    curr->__value = add_value;
    curr->__next = NULL;
    if (self->__head == NULL) {
        // map is empty
        self->__head = curr;
        curr->__prev = NULL;
    } else {
        // map is not empty
        curr->__prev = self->__tail;
        self->__tail->__next = curr;
    } 
    self->__tail = curr;
    self->__count++;
}

struct map* creat_map() {
    map* new_map = (map*)malloc(sizeof(*new_map));
    __methods* __map_methods = (__methods*)malloc(sizeof(*__map_methods));
    new_map->__head = NULL;
    new_map->__tail = NULL;
    new_map->__count = 0;
    new_map->__methods = __map_methods;
    new_map->__methods->add = &add;
    new_map->__methods->del = &del;
    new_map->__methods->dump = &dump;
    new_map->__methods->size = &size;
    new_map->__methods->creat_iter=&creat_iter;
    return new_map;
}

int main() {
    // creat
    struct map* my_map = creat_map();
    // add
    my_map->__methods->add(my_map, "a", 10);
    my_map->__methods->add(my_map, "b", 20);
    my_map->__methods->add(my_map, "c", 30);
    my_map->__methods->add(my_map, "d", 40);
    // size
    my_map->__methods->size(my_map);
    // dump
    my_map->__methods->dump(my_map);
    // iter
    printf("\nStart Iterating:\n");
    struct iter* my_iter = my_map->__methods->creat_iter(my_map);
    
    my_iter->iter_next(my_iter);

    // iter del
    my_iter->iter_del(my_iter);

    // map del
    my_map->__methods->del(my_map);
}