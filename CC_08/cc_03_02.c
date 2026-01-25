#include <stdio.h>
#include <stdlib.h>

/* 定义结构体 (Slide 3) */
struct pystr {
    int length; // 实际字符串长度
    int alloc; // 分配的内存大小
    char* data; // 指向字符串数据的指针
};

/* 构造函数 (Slide 3) */
struct pystr* pystr_new() {
    struct pystr* p = malloc(sizeof(*p));
    p->length = 0;
    p->alloc = 10;
    p->data = malloc(10);
    p->data[0] = '\0';
    return p;
}

/* 析构函数 (Slide 4) */
void pystr_del(const struct pystr* self) {
    free((void*)self->data); /* 先释放字符串内存 */
    free((void*)self); /* 再释放结构体内存 */
}

/* 调试打印函数 (Slide 4) */
void pystr_dump(const struct pystr* self) {
    printf("Pystr length=%d alloc=%d data=%s\n",
        self->length, self->alloc, self->data);
}

/* 获取长度 (Slide 4) */
int pystr_len(const struct pystr* self) {
    return self->length;
}

/* 获取字符串内容 (Slide 4) */
char* pystr_str(const struct pystr* self) {
    return self->data;
}

/* 添加单个字符 (Slide 9 - 核心逻辑) */
void pystr_append(struct pystr* self, char ch) {
    /*
     * 检查容量：如果当前长度 >= (总容量 - 2)
     * -2 是为了保留位置给即将加入的 'ch' 和必须的结尾符 '\0'
     */
    if (self->length >= (self->alloc - 2)) {
        self->alloc = self->alloc + 10; // 扩容策略：每次增加10字节
        self->data = (char*)realloc(self->data, self->alloc); // 重新分配内存
    }

    // 将字符放入当前末尾位置
    self->data[self->length] = ch;
    // 长度加 1
    self->length++;
    // 补上 C 语言字符串必须的结束符
    self->data[self->length] = '\0';
}

/* 添加字符串 (Slide 5 - 复用 pystr_append) */
void pystr_appends(struct pystr* self, char* str) {
    // 遍历输入字符串的每一个字符，复用上面的 append 函数
    int i = 0;
    while (str[i] != '\0') {
        pystr_append(self, str[i]);
        i++;
    }
}

/* 赋值字符串 (Slide 5 - 复用 pystr_appends) */
void pystr_assign(struct pystr* self, char* str) {
    // 1. 重置长度为 0 (逻辑上清空)
    self->length = 0;
    // 2. 确保第0位是结束符
    self->data[0] = '\0';
    // 3. 复用 appends 把新内容加进去
    pystr_appends(self, str);
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