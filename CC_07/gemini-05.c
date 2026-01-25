#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. 定义标签（用枚举更清晰）
enum IdentityType { IS_STUDENT = 0, IS_TEACHER = 1 };

// 2. 定义包含标签的结构体
struct Person {
    char name[20];

    // 【关键点】这个变量就是“标签”！
    // 它不在 union 里，它在外面，负责记录 union 当前的状态
    enum IdentityType type;

    // 实际存放数据的 union
    union {
        int student_id;
        char teacher_id[10];
    } id;
};

void print_person_info(struct Person *p) {
    printf("Name: %s\n", p->name);

    // 先看标签（查户口）
    if (p->type == IS_STUDENT) {
        // 标签说是学生，我们就敢放心地访问 .student_id
        printf("Type: Student, ID: %d\n", p->id.student_id);
    } else if (p->type == IS_TEACHER) {
        // 标签说是老师，我们就敢放心地访问 .teacher_id
        printf("Type: Teacher, ID: %s\n", p->id.teacher_id);
    } else {
        printf("Unknown type!\n");
    }
}