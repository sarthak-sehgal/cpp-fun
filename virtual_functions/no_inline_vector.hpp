#pragma once

class NoInlineVector
{
public:
    NoInlineVector(): x(0), y(0), z(0) {}
    NoInlineVector(int x, int y, int z): x(x), y(y), z(z) {}
    
    [[gnu::noinline]] int getX() const { return x; }
    [[gnu::noinline]] int getY() const { return y; }
    [[gnu::noinline]] int getZ() const { return z; }

    [[gnu::noinline]] void setX(int x) { this->x = x; }
    [[gnu::noinline]] void setY(int y) { this->y = y; }
    [[gnu::noinline]] void setZ(int z) { this->z = z; }
private:
    int x;
    int y;
    int z;
};
