#pragma once

#include "vector_base.hpp"

class VirtualVectorNoInline : public VectorBase
{
public:
    VirtualVectorNoInline(): x(0), y(0), z(0) {}

    __attribute__((noinline)) virtual int getX() const override { return x; }
    __attribute__((noinline)) virtual int getY() const override { return y; }
    __attribute__((noinline)) virtual int getZ() const override { return z; }

    __attribute__((noinline)) virtual void setX(int x) override { this->x = x; }
    __attribute__((noinline)) virtual void setY(int y) override { this->y = y; }
    __attribute__((noinline)) virtual void setZ(int z) override { this->z = z; }
private:
    int x;
    int y;
    int z;
};