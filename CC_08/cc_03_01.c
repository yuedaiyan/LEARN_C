#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// 前置声明
struct Point;

// 定义结构体
struct Point {
    double x;
    double y;

    // 只有“只读”操作才加 const，删除操作不加 const
    void (*del)(struct Point* self);
    void (*dump)(const struct Point* self);
    double (*origin)(const struct Point* self);
};

void point_dump(const struct Point* self){
    // 良好的习惯：检查 self 是否s空
    if (!self)
        return;
    printf("Object point@%p x=%f y=%f\n", (const void*)self, self->x, self->y);
}

void point_del(struct Point* self){
    if (!self)
        return;
    free(self); // 不需要强制转换，因为参数不再是 const
}

double point_origin(const struct Point* self){
    if (!self)
        return 0.0;
    return sqrt(self->x * self->x + self->y * self->y);
}

struct Point* point_new(double x, double y){
    // 1. 加上强制转换 (兼容 C++)
    struct Point* p = (struct Point*)malloc(sizeof(*p));

    // 2. 检查内存分配是否成功
    if (p == NULL) {
        perror("Failed to allocate memory for Point");
        return NULL;
    }

    p->x = x;
    p->y = y;
    p->dump = &point_dump; // & 可写可不写，写了更清晰
    p->origin = &point_origin;
    p->del = &point_del;
    return p;
}

int main(void){
    struct Point* pt = point_new(4.0, 5.0);

    // 必须检查对象创建是否成功
    if (pt != NULL) {
        pt->dump(pt);
        printf("Origin %f\n", pt->origin(pt));
        pt->del(pt);

        // 良好的习惯：释放后置空，防止野指针
        pt = NULL;
    }

    return 0;
}