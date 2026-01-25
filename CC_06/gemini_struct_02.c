#include <math.h> // 必须包含这个
#include <stdio.h>

// 1. 定义 Point 结构体
typedef struct {
  double x;
  double y;
} Point;
// 2. 编写计算距离的函数
double getDistance(Point p1, Point p2) {
  double distance = sqrt(pow(p2.x - p1.x,2) + pow(p2.y - p1.y,2));
  return distance;
}

int main() {
  // 3. 创建两个点并初始化
  Point p1 = {.x = 0, .y = 0};
  Point p2 = {.x = 3, .y = 4};
  double result = getDistance(p1, p2);
  // 4. 调用函数并打印结果 (使用 %f 打印 double)
  printf("result = %f", result);
  return 0;
}