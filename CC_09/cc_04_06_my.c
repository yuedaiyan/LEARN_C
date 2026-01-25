// 2026.01.13 Yi Night
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct map_iter;
struct map;

typedef struct map_node {
    char* key;
    int value;
    struct map_node* __prev;
    struct map_node* __next;
} map_node;

typedef struct map {
    map_node* __head;
    map_node* __tail;
    int count;
    void (*put)(struct map* self, char* put_char, int put_int);
    int (*size)(struct map* self);
    void (*del)(struct map* self);
    struct map_iter* (*iter)(struct map* self);
} map;

/*__________________________________*/
typedef struct map_iter {
    struct map_node* curr;
    struct map_node* (*next)(struct map_iter* self);
    void (*del)(struct map_iter* self);
} map_iter;
/*__________________________________*/
int func_size(struct map* self){
    return self->count;
}
void iter_func_del(struct map_iter* self) {
    free(self);
}

void func_del(struct map* self){
    map_node* curr,*next;
    curr = self->__head;
    while(curr){
        free(curr->key);
        next = curr->__next;
        free(curr);
        curr = next;
    }
    free(self);
    printf("del sucess.\n");
}
void func_put(struct map* self, char* add_key, int add_value) {
    map_node* curr = (map_node*)malloc(sizeof(*curr));
    curr->key = strdup(add_key);
    curr->value = add_value;
    curr->__next = NULL;
    if (self->__head == NULL) {
        self->__head = curr;
    }else{
        self->__tail->__next = curr;
    }

    curr->__prev = self->__tail;
    
    self->__tail = curr;
    self->count++;

}

struct map_node* iter_func_next(struct map_iter* self) {
    map_node* c = self->curr;
    self->curr = self->curr->__next;
    if (self->curr==NULL){return NULL;}
    return c;
}

struct map_iter* new_iter(map*self){
    map_iter* curr_iter = (map_iter*)malloc(sizeof(*curr_iter));
    curr_iter->curr = self->__head;
    curr_iter->del = &iter_func_del;
    curr_iter->next = &iter_func_next;
    return curr_iter;
}

struct map* new_map() {
    struct map* new_map = (map*)malloc(sizeof(*new_map));
    new_map->__head = NULL;
    new_map->__tail = NULL;
    new_map->count = 0;
    new_map->put = &func_put;
    new_map->size = &func_size;
    new_map->del = &func_del;
    new_map->iter = &new_iter;// !
    return new_map;
}

/*__________________________________*/
int main() {
    struct map* my_map = new_map();
    my_map->put(my_map, "z", 8);
    my_map->put(my_map, "y", 2);
    my_map->put(my_map, "b", 3);
    my_map->put(my_map, "a", 4);
    printf("size: %d\n", my_map->size(my_map));

    // ... 在 printf size 之后 ...

    printf("\nStart Iterating:\n");
    // 1. 获取迭代器
    struct map_iter* my_iter = my_map->iter(my_map);
    map_node* cur_node;

    // 2. 循环调用
    while (1) {
        cur_node = my_iter->next(my_iter);
        if (cur_node == NULL)
            break; // 终结条件
        printf("Key: %s, Value: %d\n", cur_node->key, cur_node->value);
    }

    // 3. 别忘了释放迭代器
    my_iter->del(my_iter);

    my_map->del(my_map);
}