#include <cmath> // 1. 使用 C++ 标准数学库
#include <iostream>

class Point {
private: // 2. 封装：数据成员私有化
  double x, y;

public:
  // 3. 构造函数初始化列表 (Initializer List)
  // 这里的 : x(x), y(y) 是 C++ 特有的高效初始化方式
  Point(double x, double y) : x(x), y(y) {
    // 函数体为空
  }

  // 4. const 修饰符：表示该函数不会修改对象的状态
  void dump() const {
    // 5. 使用 C++ 的 std::cout 替代 printf
    std::cout << "Object point x=" << x << " y=" << y << std::endl;
  }

  // const 修饰符
  double origin() const { return std::sqrt(x * x + y * y); }
};

int main() {
  Point pt(4.0, 5.0);
  pt.dump();

  // std::cout 输出风格
  std::cout << "Origin: " << pt.origin() << std::endl;

  return 0;
}