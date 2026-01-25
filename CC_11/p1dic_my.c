// 2026.01.21 Yi 14:08
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char* key;
    char* val;
} node;

typedef struct dic {
    size_t length;
    size_t alloc;
    // length < alloc
    node* items;
} dic;

dic* dic_make() {
    dic* new_dic = (dic*)malloc(sizeof(*new_dic));
    new_dic->length = 0;
    new_dic->alloc = 2;

    new_dic->items = (node*)malloc(sizeof(*new_dic->items) * new_dic->alloc);
    for (size_t i = 0; i < new_dic->alloc; i++) {
        new_dic->items[i].key = NULL;
        new_dic->items[i].val = NULL;
    }
    return new_dic;
}

unsigned long aux_hash(char* str) {
    unsigned long rethash = 5381;
    int i;
    while ((i = *str++)) {
        rethash = (rethash << 5) + rethash + i;
    }

    return rethash;
}

node* aux_get(dic* self, char* key_get) {
    size_t key_mod = aux_hash(key_get) % self->alloc; // get key's mod

    while (1) {
        if (self->items[key_mod].key == NULL) {
            return &self->items[key_mod];
        }
        if (strcmp(self->items[key_mod].key, key_get) == 0) {
            return &self->items[key_mod];
        }
        if (key_mod == 0) {
            key_mod = self->alloc - 1;
        } else {
            key_mod--;
        }
    }
}
// return node->key==NULL：key_get不存在于列表之中
// return node->key!=NULL: 相同的key_get已经存在于列表之中

int aux_alloc_check(dic* self) {
    if (self->alloc == (self->length + 1) || self->alloc * 2 <= self->length * 3) {
        printf("检测到 Dic 需要扩容\n");
        printf("    当前：   length：%zu，alloc：%zu\n", self->length, self->alloc);

        size_t old_alloc = self->alloc; // save old alloc
        node* old_items = self->items; // save old items
        self->alloc = self->alloc * 2;

        self->items = (node*)malloc(sizeof(*self->items) * self->alloc);
        // 搬运数据：
        //      先统一填装 NULL
        //      之后搬运旧节点
        for (size_t i = 0; i < self->alloc; i++) {
            self->items[i].key = NULL;
            self->items[i].val = NULL;
        }

        for (size_t i = 0; i < old_alloc; i++) {
            if (old_items[i].key == NULL) {
                // 旧表中的这个节点为NULL → 直接跳过
                continue;
            }
            node* curr = aux_get(self, old_items[i].key);
            curr->key = old_items[i].key;
            curr->val = old_items[i].val;
            
        }
        free(old_items);
        printf("    扩容后： length：%zu，alloc：%zu\n", self->length, self->alloc);
        return 1;
    }
    return 0;
}
// return 1: 重新malloc
// return 0：没有执行malloc

void aux_strcpy(node* target_node,char*key_add,char*val_add){
    char* key_mal = (char*)malloc(strlen(key_add) + 1);
    strcpy(key_mal, key_add);
    char* val_mal = (char*)malloc(strlen(val_add) + 1);
    strcpy(val_mal, val_add);
    target_node->key = key_mal;
    target_node->val = val_mal;
}

void dic_add(dic* self, char* key_add, char* val_add) {
    // 1.先get一下key，看看是要修改现有节点的val，还是mal新节点
    //      1.1. 如果新改就直接修改
    //      1.2.如果需要mal，
    //          1.2.1.先检查空间
    //          1.2.2.添加节点

    node* curr = aux_get(self, key_add);
    if (curr->key) {
        // not NULL
        free(curr->val);
        curr->val = (char*)malloc(strlen(val_add) + 1);
        strcpy(curr->val, val_add);

    } else {
        // is NULL
        if (aux_alloc_check(self)) {
            // new malloc
            aux_strcpy(aux_get(self, key_add), key_add, val_add);
            self->length++;
        } else {
            // didn't malloc this time
            aux_strcpy(curr, key_add, val_add);
            self->length++;
        }
    }
}

void dic_dump(dic* self){
    printf("Dic length:%zu, malloc:%zu\n", self->length, self->alloc);
    for (size_t i = 0; i < self->alloc; i++) {
        if(self->items[i].key==NULL){
            // 当前 node 的 key 是 NULL
            printf("|    [%zu] (empty)\n",i);

        }else{
            // 当前 node 的 key 不为NULL
            printf("|    [%zu] key:%s, value:%s\n", i, self->items[i].key, self->items[i].val);
        }
    }
    printf("Dic dump over\n");
}

char* dic_get(dic*self,char*key_get){
    node* ret_node = aux_get(self, key_get);
    if(ret_node->key==NULL){
        return NULL;
    }else{
        return ret_node->val;
    }
}

void dic_del(dic*self){

    for (size_t i = 0; i < self->alloc;i++){
        if(self->items[i].key!=NULL){
            free(self->items[i].key);
            free(self->items[i].val);
        }

    }
    free(self->items);
    free(self);
    printf("Dic deled\n");
}

int main() {

    dic* Dic = dic_make();
    printf("\n");

    // 1. 插入数据
    printf("Inserting 'z'...\n");
    dic_add(Dic, "z", "Catch phrase");
    dic_dump(Dic);
    // 注意：由于初始容量是2，插入第2个元素时如果不扩容，表就满了(100%)。
    // 为了安全，上面的代码修改为更积极的扩容，所以这里插入 'y' 时可能就会触发扩容。
    printf("\n");
    printf("Inserting 'y'...\n");
    dic_add(Dic, "y", "B");
    dic_dump(Dic);

    // 2. 插入更多数据
    printf("\n");
    printf("Inserting 'c'...\n");
    dic_add(Dic, "c", "42");
    dic_dump(Dic);

    printf("\n");
    dic_add(Dic, "a", "Apple");
    dic_add(Dic, "b", "Banana");

    dic_dump(Dic);

    // 4. 测试读取
    printf("Value for 'z': %s\n", dic_get(Dic, "z"));
    char* val_c = dic_get(Dic, "c");
    printf("Value for 'c': %s\n", val_c ? val_c : "NULL");

    // 测试读取不存在的键 (如果扩容逻辑不对，这里会死循环)
    printf("Value for 'missing': %s\n", dic_get(Dic, "missing_key") ? "Found" : "NULL");

    dic_del(Dic);
    return 0;
}