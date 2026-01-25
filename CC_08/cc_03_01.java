import java.lang.Math;

// 去掉这里的 public，因为一个文件只能有一个 public 类
class Point {
    double x, y;

    Point(double x, double y) {
        this.x = x;
        this.y = y;
    }

    void dump() {
        // %s 会调用 this.toString()，默认打印类名@哈希值
        System.out.printf("%s x=%f y=%f\n", this.getClass().getSimpleName(), this.x, this.y);
    }

    double origin() {
        // 也可以使用 Math.hypot(x, y) 更简洁
        return Math.sqrt(this.x * this.x + this.y * this.y);
    }
}

public class cc_03_01 {
    public static void main(String[] args) {
        Point pt = new Point(4.0, 5.0);
        pt.dump();
        System.out.printf("Origin %f\n", pt.origin());
    }
}