#pragma once

class InlineVector
{
public:
    InlineVector(): x(0), y(0), z(0) {}
    InlineVector(int x, int y, int z): x(x), y(y), z(z) {}

    __attribute__((always_inline)) int getX() const { return x; }
    __attribute__((always_inline)) int getY() const { return y; }
    __attribute__((always_inline)) int getZ() const { return z; }

    __attribute__((always_inline)) void setX(int x) { this->x = x; }
    __attribute__((always_inline)) void setY(int y) { this->y = y; }
    __attribute__((always_inline)) void setZ(int z) { this->z = z; }
private:
    int x;
    int y;
    int z;
};
