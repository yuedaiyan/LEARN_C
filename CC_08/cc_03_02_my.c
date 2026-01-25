#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MALLOCSTEP 64
typedef struct pystr {
    int length; // 实际字符串长度
    int alloc; // 分配的内存大小
    char* data; // 指向字符串数据的指针
} pystr;

/* 构造函数 (Slide 3) */
struct pystr* pystr_new() {
    pystr* np = (pystr*)malloc(sizeof(*np));
    if (np == NULL)
        return NULL;
    np->length = 0;
    np->alloc = MALLOCSTEP;
    char* n_data = (char*)malloc(MALLOCSTEP);
    np->data = n_data;
    np->data[0] = '\0';
    return np;
}
/* 析构函数 (Slide 4) */
void pystr_del(pystr* self) {
    free(self->data);
    free(self);
    printf("pystr_del success!\n");
}

/* 调试打印函数 (Slide 4) */
void pystr_dump(pystr* self) {
    printf("length:%d, alloc:%d, data:%s\n", self->length, self->alloc, self->data);
}

/* 获取长度 (Slide 4) */
int pystr_len(pystr* self) {
    return self->length;
}

/* 获取字符串内容 (Slide 4) */
char* pystr_str(pystr* self) {
    return self->data;
}

/* 添加单个字符 (Slide 9 - 核心逻辑) */
void pystr_append(pystr* self, char ch) {
    // more arm
    if (self->length >= self->alloc - 2) {
        self->alloc = self->alloc + MALLOCSTEP;
        void* temp = realloc(self->data, self->alloc);
        if (temp != NULL) {
            self->data = temp;
        } else {
            printf("Alloc ARM error\n");
            free(self->data);
            return;
        }
    }
    // add
    self->data[self->length] = ch;
    self->length++;
    self->data[self->length] = '\0';
}

/* 添加字符串 (Slide 5 - 复用 pystr_append) */
void pystr_appends(pystr* self, char* char_str) {
    int i = 0;
    while (char_str[i] != '\0') {
        pystr_append(self, char_str[i]);
        i++;
    }
}

/* 赋值字符串 (Slide 5 - 复用 pystr_appends) */
void pystr_assign(pystr* self, char* char_n) {
    self->length = 0;
    self->alloc = MALLOCSTEP;
    self->data = realloc(self->data, self->alloc);
    self->data[0]='\0';
    pystr_appends(self, char_n);
}
/* 主函数测试 (Slide 2/10) */
int main(void) {
    struct pystr* x = pystr_new();
    pystr_dump(x); // 初始状态

    pystr_append(x, 'H');
    pystr_dump(x); // 应该有 H

    pystr_appends(x, "ello world");
    pystr_dump(x); // 此时会触发 realloc，因为长度超过了初始的10

    pystr_assign(x, "A completely new string");
    printf("String = %s\n", pystr_str(x));
    printf("Length = %d\n", pystr_len(x));

    pystr_del(x); // 清理内存

    return 0;
}