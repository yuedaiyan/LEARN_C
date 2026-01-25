#include <iostream>
using namespace std;

// 1. 父类：普通怪兽
class Monster {
public:
    // 【Public】：任何人都能调用
    // 比如：游戏主引擎让怪兽报上名来
    void introduce() {
        cout << "我是怪兽，我有 " << hp << " 血量。" << endl;
    }

private:
    // 【Private】：只有 Monster 自己知道
    // 比如：怪兽的唯一身份证号，连子类都不能改
    int id_number = 999;

protected:
    // 【Protected】：Monster 自己和它的子类(Boss)都知道
    // 比如：血量。因为 Boss 可能有技能需要修改自己的血量，
    // 但我们不希望 main 函数直接把血量改成 0。
    int hp = 100;
};

// 2. 子类：Boss 怪兽（继承自 Monster）
class Boss : public Monster {
public:
    void specialAttack() {
        // --- 访问 Protected ---
        // 可以访问！因为 Boss 是 Monster 的孩子。
        hp = hp - 10; // 发大招自损 10 点血
        cout << "Boss 发动大招！剩余血量: " << hp << endl;

        // --- 访问 Private ---
        // 报错！不可以访问！
        // 即使是亲儿子，也不能碰老子的身份证号。
        // id_number = 888; // 这一行如果取消注释，编译会报错
    }
};

// 3. 外部世界：main 函数
int main() {
    Monster m;
    Boss b;

    // --- 访问 Public ---
    m.introduce(); // 没问题，这是公开接口
    b.introduce(); // 没问题，子类继承了公开接口

    // --- 访问 Protected ---
    // 报错！外部人员（main函数）不能碰受保护数据
    // m.hp = 0;   // 编译报错！
    // b.hp = 0;   // 编译报错！

    // --- 访问 Private ---
    // 报错！更不可能碰私有数据
    // m.id_number = 0; // 编译报错！

    // Boss 发动技能（内部修改了 hp）
    b.specialAttack(); // 没问题，这是通过 Public 方法间接修改 Protected 数据

    return 0;
}