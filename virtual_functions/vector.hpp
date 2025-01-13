#pragma once

#ifdef INLINE
    #define INLINE_ATTR [[gnu::always_inline]]
#else
    #define INLINE_ATTR [[gnu::noinline]]
#endif

class Vector
{
public:
    INLINE_ATTR int getX() const { return x; }
    INLINE_ATTR int getY() const { return y; }
    INLINE_ATTR int getZ() const { return z; }

    INLINE_ATTR void setX(int x) { this->x = x; }
    INLINE_ATTR void setY(int y) { this->y = y; }
    INLINE_ATTR void setZ(int z) { this->z = z; }
private:
    int x = 0;
    int y = 0;
    int z = 0;
};
