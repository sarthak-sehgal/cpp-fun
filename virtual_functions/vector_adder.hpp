#include "profiler.hpp"

#include <array>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

template <typename PointerType, typename ObjectType>
struct VectorAdder
{
    auto run()
    {
        // allocate new vector objects in each test run
        // to avoid cache effects
        std::array<PointerType*, 1000> vectorArr;
        ObjectType result;
        std::size_t vectorIdx = 0;

        for (auto j = 0u; j < 1000; ++j)
        {
            vectors[vectorIdx++] = std::shared_ptr<PointerType>(new ObjectType());
            auto* v = vectors[vectorIdx-1].get();
            v->setX(dist(rng));
            v->setY(dist(rng));
            v->setZ(dist(rng));
            vectorArr[j] = v;
        }

        int x = 0;
        int y = 0;
        int z = 0;
        profiler.reset();
        for (auto j = 0u; j < 1000; ++j)
        {
            x += vectorArr[j]->getX();
            y += vectorArr[j]->getY();
            z += vectorArr[j]->getZ();
        }
        profiler.lap();

        result.setX(x);
        result.setY(y);
        result.setZ(z);

        std::cout << "Result: " << result.getX() << ' ' << result.getY() << ' ' << result.getZ() << '\n';

        return profiler.get();
    }

private:
    std::random_device dev;
    std::mt19937 rng = std::mt19937(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist = std::uniform_int_distribution<std::mt19937::result_type>(1, 1000);
    Profiler profiler;
    std::array<std::shared_ptr<PointerType>, 1000> vectors;
};