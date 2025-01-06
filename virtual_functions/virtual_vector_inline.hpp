#pragma once

#include "vector_base.hpp"

class VirtualVectorInline : public VectorBase
{
public:
    VirtualVectorInline(): x(0), y(0), z(0) {}

    __attribute__((always_inline)) virtual int getX() const override { return x; }
    __attribute__((always_inline)) virtual int getY() const override { return y; }
    __attribute__((always_inline)) virtual int getZ() const override { return z; }

    __attribute__((always_inline)) virtual void setX(int x) override { this->x = x; }
    __attribute__((always_inline)) virtual void setY(int y) override { this->y = y; }
    __attribute__((always_inline)) virtual void setZ(int z) override { this->z = z; }
private:
    int x;
    int y;
    int z;
};