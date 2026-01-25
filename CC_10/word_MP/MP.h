#pragma once
typedef struct _node {
    char* key;
    int value;
    struct _node* _next;
    struct _node* _prev;
    struct _node* _left;
    struct _node* _right;
} _node;

typedef struct TM {
    int _count;
    struct _iter* _iter;
    struct _node* _head;
    struct _node* _tail;
    struct _node* _root;
    struct methods* methods;
} TM;

typedef struct _iter {
    _node* _current;
    void (*_del)(struct _iter* self);
} _iter;

typedef struct methods {
    void (*TM_size)(TM* self);
    void (*TM_add)(TM* self, char* key_add, int value_add);
    void (*TM_iter)(TM* self);
    _iter* (*TM_iter_make)(TM* self);
    void (*TM_chain_dump)(TM* self);
    void (*TM_del)(TM* self);
    void (*TM_tree_dump)(TM* self);
    void (*TM_iter_del)(TM* self);
    int (*TM_get)(TM* self, char* key_target, int error_index);
} methods;

TM* TM_create();