#pragma once

class VectorBase
{
public:
    virtual int getX() const = 0;
    virtual int getY() const = 0;
    virtual int getZ() const = 0;

    virtual void setX(int x) = 0;
    virtual void setY(int y) = 0;
    virtual void setZ(int z) = 0;

    virtual ~VectorBase() = default;
};