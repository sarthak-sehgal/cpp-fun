#include "profiler.hpp"

#include <iostream>
#include <memory>
#include <random>
#include <vector>

template <typename PointerType, typename ObjectType>
struct VectorAdder
{
    auto run()
    {
        for (auto i = 0u; i < 1000; ++i)
        {
            // allocate new vector objects in each test run
            // to avoid cache effects
            std::array<PointerType*, 1000> vectorArr;
            ObjectType result;

            for (auto j = 0u; j < 1000; ++j)
            {
                vectors.push_back(std::shared_ptr<PointerType>(new ObjectType()));
                auto* v = vectors.back().get();
                v->setX(dist(rng));
                v->setY(dist(rng));
                v->setZ(dist(rng));
                vectorArr[j] = v;
            }

            profiler.reset();
            for (auto j = 0u; j < 1000; ++j)
            {
                result.setX(result.getX() + vectorArr[j]->getX());
                result.setY(result.getY() + vectorArr[j]->getY());
                result.setZ(result.getZ() + vectorArr[j]->getZ());
            }
            profiler.lap();
            std::cout << "Result: " << result.getX() << ' ' << result.getY() << ' ' << result.getZ() << '\n';
        }

        return profiler.get();
    }

private:
    std::random_device dev;
    std::mt19937 rng = std::mt19937(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist = std::uniform_int_distribution<std::mt19937::result_type>(1, 1000);
    Profiler profiler;
    std::vector<std::shared_ptr<PointerType>> vectors;
};