#pragma once

class NoInlineVector
{
public:
    NoInlineVector(): x(0), y(0), z(0) {}
    NoInlineVector(int x, int y, int z): x(x), y(y), z(z) {}
    
    __attribute__((noinline)) int getX() const { return x; }
    __attribute__((noinline)) int getY() const { return y; }
    __attribute__((noinline)) int getZ() const { return z; }

    __attribute__((noinline)) void setX(int x) { this->x = x; }
    __attribute__((noinline)) void setY(int y) { this->y = y; }
    __attribute__((noinline)) void setZ(int z) { this->z = z; }
private:
    int x;
    int y;
    int z;
};
