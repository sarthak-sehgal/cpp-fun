#pragma once

#ifdef INLINE
    #define INLINE_ATTR [[gnu::always_inline]]
#else
    #define INLINE_ATTR [[gnu::noinline]]
#endif


#include "vector_base.hpp"

class VirtualVector_1 : public VectorBase
{
public:
    VirtualVector_1(): x(0), y(0), z(0) {}

    INLINE_ATTR virtual int getX() const override { return x; }
    INLINE_ATTR virtual int getY() const override { return y; }
    INLINE_ATTR virtual int getZ() const override { return z; }

    INLINE_ATTR virtual void setX(int x) override { this->x = x; }
    INLINE_ATTR virtual void setY(int y) override { this->y = y; }
    INLINE_ATTR virtual void setZ(int z) override { this->z = z; }
private:
    int x;
    int y;
    int z;
};