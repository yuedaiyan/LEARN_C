// 2026.01.22 Yi 03:34
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char* key;
    char* val;
} node;

typedef struct dic {
    int alloc;
    int length;
    node* items;
    int* index;
} dic;

void dic_dumpByNodeChain(dic* self);
void dic_dumpByHashList(dic* self);
void dic_dumpByItemsList(dic* self);

void aux_mallocReSet(dic* self) {
    printf("    aux → [int aux_mallocReSet] called.\n");
    self->items = (node*)malloc(sizeof(*self->items) * self->alloc);
    self->index = (int*)malloc(sizeof(*self->index) * self->alloc);
    for (int i = 0; i < self->alloc; i++) {
        self->items[i].key = NULL;
        self->items[i].val = NULL;
        self->index[i] = -1;
    }
}

dic* dic_make() {
    printf("[dic* dic_make] called\n");
    dic* new_dic = (dic*)malloc(sizeof(*new_dic));
    new_dic->alloc = 2;
    new_dic->length = 0;
    aux_mallocReSet(new_dic);
    return new_dic;
}

unsigned long aux_mod(dic* self, char* str) {
    printf("    aux → [int aux_mod] called: %s → ", str);
    unsigned long hash = 5381;
    int i;
    while ((i = *str++)) {
        hash = (hash << 5) + hash + i;
    }
    unsigned long retval = hash & (self->alloc - 1);
    printf("%ld\n", retval);
    return retval;
}

int aux_mallocCheck(dic* self) {
    printf("    aux → [int aux_mallocCheck] called: malloc: %d length: %d\n", self->alloc, self->length);
    if (self->alloc * 2 < self->length * 3 || self->alloc == self->length + 1) {
        printf("result: remalloc\n");
        int old_alloc = self->alloc;
        node* old_items = self->items;
        int* old_index = self->index;
        int old_length = self->length;

        self->alloc = self->alloc * 2;

        aux_mallocReSet(self);

        self->length = old_length;
        for (int i = 0; i < old_length; i++) {
            unsigned long mod = aux_mod(self, old_items[i].key);
            for (int p = 0; p < self->alloc; p++) {

                if (self->index[mod] == -1) {
                    // is -1,means index is empty
                    self->items[i].key = old_items[i].key;
                    self->items[i].val = old_items[i].val;
                    self->index[mod] = i;
                    break;
                }
                if (mod == 0) {
                    mod = self->alloc - 1;
                } else {
                    mod--;
                }
            }
        }

        free(old_index);
        free(old_items);

        dic_dumpByHashList(self);
        return 1;
    } else {
        printf("reslult: pass\n");
        return 0;
    }
}

void aux_trueAdd(dic* self, char* key_add, char* val_add, unsigned long mod_Key) {
    printf("    aux → [void aux_trueAdd] called: key: %s → mod: %ld value: %s\n", key_add, mod_Key, val_add);

    char* key_malloc = (char*)malloc(strlen(key_add) + 1);
    char* val_malloc = (char*)malloc(strlen(val_add) + 1);

    strcpy(key_malloc, key_add);
    strcpy(val_malloc, val_add);

    self->items[self->length].key = key_malloc;
    self->items[self->length].val = val_malloc;

    self->index[mod_Key] = self->length;
    self->length++;

    return;
}

void dic_add(dic* self, char* key_add, char* val_add) {
    printf("[void dic_add] called: %s – %s\n", key_add, val_add);
    aux_mallocCheck(self);
    unsigned long mod = aux_mod(self, key_add);
    // have some thind on the index palce
    for (int i = 0; i < self->alloc; i++) {
        int idx = self->index[mod];
        if (idx == -1) {
            aux_trueAdd(self, key_add, val_add, mod);
            return;
        }
        if (strcmp(self->items[idx].key, key_add) == 0) {
            // same
            char* curr = (char*)malloc(strlen(val_add) + 1);
            strcpy(curr, val_add);
            free(self->items[idx].val);
            self->items[idx].val = curr;
            return;
        }
        if (mod == 0) {
            mod = self->alloc - 1;
        } else {
            mod--;
        }
    }
}

void dic_dumpByHashList(dic* self) {
    printf("[void dic_dumpByHashList] called\n");
    for (int i = 0; i < self->alloc; i++) {
        if (self->index[i] == -1) {
            printf("    [%d] key: (null), value: (null)\n", i);
            continue;
        }
        node* curr = &self->items[self->index[i]];
        printf("    [%d] key: %s, value: %s\n", i, curr->key, curr->val);
    }
}

void dic_dumpByItemsList(dic* self) {
    printf("[void dic_dumpByItemsList] called\n");
    for (int i = 0; i < self->length; i++) {
        printf("    [%d] key: %s, value: %s\n", i, self->items[i].key, self->items[i].val);
    }
}

char* dic_get(dic* self, char* key_get) {
    printf("[dic* dic_get] called: %s\n", key_get);
    unsigned long mod = aux_mod(self, key_get);

    for (int i = 0; i < self->alloc; i++) {
        int idx = self->index[mod];

        if (idx == -1) {
            return NULL;
        }
        printf("    call times:[%d] mod: %ld key: %s\n", i, mod, self->items[idx].key);
        if (strcmp(self->items[idx].key, key_get) == 0) {
            return self->items[idx].val;
        }

        if (mod == 0) {
            mod = self->alloc - 1;
        } else {
            mod--;
        }
    }
    return NULL;
}

void dic_del(dic* self) {
    printf("[dic* dic_del] called.\n");
    for (int i = 0; i < self->length; i++) {
        free(self->items[i].key);
        free(self->items[i].val);
    }
    free(self->items);
    free(self->index);
    free(self);
}

void dic_dumpByNodeChain(dic* self) {
    printf("[void dic_dumpByNodeChain] called\n");
    for (int i = 0; i < self->length; i++) {
        printf("    [%d] key: %s, value: %s\n", i, self->items[i].key, self->items[i].val);
    }
}

int main() {
    printf("\n--- step 1:make ---\n");

    dic* Dic = dic_make();

    printf("\n--- step 2:add ---\n");
    printf("\nadd z – Catch phrase\n");
    dic_add(Dic, "z", "Catch phrase");
    dic_dumpByHashList(Dic);
    dic_dumpByNodeChain(Dic);

    printf("\nadd z – W\n");
    dic_add(Dic, "z", "W");
    dic_dumpByHashList(Dic);
    dic_dumpByNodeChain(Dic);

    printf("\nadd SAKAI – B\n");
    dic_add(Dic, "SAKAI", "B");
    dic_dumpByHashList(Dic);
    dic_dumpByNodeChain(Dic);

    printf("\nadd sally – C\n");
    dic_add(Dic, "sally", "C");
    dic_dumpByHashList(Dic);
    dic_dumpByNodeChain(Dic);

    printf("\nadd a – D\n");
    dic_add(Dic, "a", "D");
    dic_dumpByHashList(Dic);
    dic_dumpByNodeChain(Dic);

    printf("\n--- step 3:get ---\n");
    printf("Value for 'z': %s\n", dic_get(Dic, "z"));
    char* val_c = dic_get(Dic, "c");
    printf("Value for 'c': %s\n", val_c ? val_c : "NULL");
    // 测试读取不存在的键 (如果扩容逻辑不对，这里会死循环)
    printf("Value for 'missing_key': %s\n", dic_get(Dic, "missing_key") ? "Found" : "NULL");

    printf("\n--- step 4:del ---\n");
    dic_del(Dic);

    printf("\n[int main] end\n");
    return 0;
}