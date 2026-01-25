<?php  // <--- 必须加上这一行

class Point {
    // PHP 7.4+ 支持属性类型声明
    private float $x;
    private float $y;

    public function __construct(float $x, float $y) {
        $this->x = $x;
        $this->y = $y;
    }

    public function getDescription(): string {
        return sprintf("Object point x=%.2f y=%.2f", $this->x, $this->y);
    }

    public function getDistance(): float {
        return sqrt($this->x * $this->x + $this->y * $this->y);
    }
}

$pt = new Point(4.0, 5.0);

echo $pt->getDescription() . "\n";
printf("Distance from origin: %.2f\n", $pt->getDistance());