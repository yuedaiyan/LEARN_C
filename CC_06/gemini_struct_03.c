#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char name[20];
  float price;
} Book;

int main() {
  Book book[3];
  for (int i = 0; i < 3; i++) {
    printf("\ninput book[%d]'s name:", i + 1);
    fgets(book[i].name, 20, stdin);
    book[i].name[strcspn(book[i].name, "\n")] = '\0';
    // book[i].name[strcspn(book[i].name, "\n")] = 0;
    printf("input book[%d]'s price:", i + 1);
    scanf("%f", &book[i].price);
    while (getchar() != '\n')
      ;
  }

  int max_price_index = 0;
  for (int i = 0; i < 3; i++) {
    if (book[i].price > book[max_price_index].price) {
      max_price_index = i;
    }
  }
  printf("\nbook: %s max_price: %.1f", book[max_price_index].name,
         book[max_price_index].price);

  // 在你的代码末尾
  // 1. 定义一个 Book 类型的指针，指向最贵的那本书
  Book *p = &book[max_price_index];

  // 2. 使用指针和 -> 符号打印
  printf("\n通过指针访问：\n");
  printf("书名: %s, 价格: %.1f\n", p->name, p->price);

  return 0;
}