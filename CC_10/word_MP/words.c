#include "MP.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_01() {
    TM* my_TM = TM_create();
    printf("1. Basic Insertions (h, d, b, f)\n");
    my_TM->methods->TM_add(my_TM, "h", 22);
    my_TM->methods->TM_add(my_TM, "h", 42); // Replace (更新)
    my_TM->methods->TM_add(my_TM, "d", 8);
    my_TM->methods->TM_add(my_TM, "b", 123);
    my_TM->methods->TM_add(my_TM, "f", 6);

    my_TM->methods->TM_chain_dump(my_TM);

    printf("2. More Insertions (k, m, j) - Filling gaps\n");
    my_TM->methods->TM_add(my_TM, "k", 9);
    my_TM->methods->TM_add(my_TM, "m", 67);
    my_TM->methods->TM_add(my_TM, "j", 12);

    printf("\n3. Edge Cases (Insert smaller than head, insert larger than tail)\n");
    my_TM->methods->TM_add(my_TM, "a", 1); // New Head (比 b 小)
    my_TM->methods->TM_add(my_TM, "z", 999); // New Tail (比 m 大)

    my_TM->methods->TM_size(my_TM);

    my_TM->methods->TM_chain_dump(my_TM);
    // 最终顺序: a -> b -> d -> f -> h -> j -> k -> m -> z

    my_TM->methods->TM_tree_dump(my_TM);

    printf("\n4. Iter print test\n");
    int i = 0;
    my_TM->_iter = my_TM->methods->TM_iter_make(my_TM);

    while (my_TM->_iter->_current) {
        i++;
        printf("%d key: %s, value: %d\n", i, my_TM->_iter->_current->key, my_TM->_iter->_current->value);
        my_TM->methods->TM_iter(my_TM);
    }
    my_TM->methods->TM_iter_del(my_TM);

    printf("\n5. Lookup Test\n");
    printf("Value of 'h': %d\n", my_TM->methods->TM_get(my_TM, "h", -1));
    printf("Value of 'z': %d\n", my_TM->methods->TM_get(my_TM, "z", -1));
    printf("Value of 'x' (missing): %d\n", my_TM->methods->TM_get(my_TM, "x", -1));

    printf("\n6. destroy MT\n");
    my_TM->methods->TM_del(my_TM);

    printf("\n--- main Completed ---\n");
}

void words() {
    printf("1.Create words_TM\n");
    TM* words_TM = TM_create();

    // char name[512];
    // printf("Enter file name:\n");
    // scanf("%s",name);

    char name[512] = "words.txt";

    FILE* file_ptr = fopen(name, "r");
    if (file_ptr == NULL) {
        printf("input name error\n");
        return;
    } else {
        printf("%s input success\n", name);
    }

    char word[128];
    while (fscanf(file_ptr, "%s", word) != EOF) {
        // printf("\n%d input: %s\n", words_count, word);

        int v_cnt = 0;
        for (int c_cnt = 0; word[c_cnt] != '\0'; c_cnt++) {
            if (isalpha(word[c_cnt]) == 0) {
                // 如果当前字符 不是字母
                continue;
            } else {
                word[v_cnt] = tolower(word[c_cnt]);
                v_cnt++;
            }
        }
        word[v_cnt] = '\0';

        if (word[0] == '\0') {
            continue;
        } else {
            int count = words_TM->methods->TM_get(words_TM, word, 0);
            words_TM->methods->TM_add(words_TM, word, count + 1);
        }
    }

    fclose(file_ptr);
    words_TM->methods->TM_chain_dump(words_TM);
    words_TM->methods->TM_tree_dump(words_TM);

    // 寻找频率最多单词
    words_TM->_iter = words_TM->methods->TM_iter_make(words_TM);
    _node* max_count_node = words_TM->_iter->_current;
    while (words_TM->_iter->_current) {
        if (max_count_node->value < words_TM->_iter->_current->value) {
            max_count_node = words_TM->_iter->_current;
        }
        words_TM->methods->TM_iter(words_TM);
    }
    printf("\nMAX word:%s, count:%d\n", max_count_node->key, max_count_node->value);
    words_TM->methods->TM_iter_del(words_TM);

    words_TM->methods->TM_del(words_TM);
}

int main() {
    // test_01();
    words();
    return 0;
}
