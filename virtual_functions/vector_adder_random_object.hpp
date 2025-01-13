#include "profiler.hpp"

#include <iostream>
#include <memory>
#include <random>
#include <vector>

template <typename PointerType>
std::shared_ptr<PointerType> createRandomObject(std::size_t)
{
    return nullptr;
}

template <typename PointerType, typename T, typename... Rest>
std::shared_ptr<PointerType> createRandomObject(std::size_t index) {
    if (index == 0) {
        return std::shared_ptr<PointerType>(new T());
    } else {
        return createRandomObject<PointerType, Rest...>(index-1);
    }
}

template <typename PointerType, typename... ObjectType>
std::shared_ptr<PointerType> createRandomObject()
{
    #ifdef PATTERN
        static std::size_t i = 0;
        std::size_t random_index = (i++) % sizeof...(ObjectType);
    #else
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, sizeof...(ObjectType) - 1);
        int random_index = dis(gen);
    #endif

    return createRandomObject<PointerType, ObjectType...>(random_index);
}

template <typename PointerType, typename... ObjectType>
struct VectorAdderRandomObject
{
    auto run()
    {
        // allocate new vector objects in each test run
        // to avoid cache effects
        std::array<PointerType*, 1000> vectorArr;
        std::size_t vectorIdx = 0;

        for (auto j = 0u; j < 1000; ++j)
        {
            vectors[vectorIdx++] = createRandomObject<PointerType, ObjectType...>();
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

        std::cout << "Result: " << x << ' ' << y << ' ' << z << '\n';

        return profiler.get();
    }

private:
    std::random_device dev;
    std::mt19937 rng = std::mt19937(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist = std::uniform_int_distribution<std::mt19937::result_type>(1, 1000);
    Profiler profiler;
    std::array<std::shared_ptr<PointerType>, 1000> vectors;
};