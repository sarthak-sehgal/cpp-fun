#include "vector_adder.hpp"
#include "vector_adder_random_object.hpp"
#include "vector.hpp"
#include "virtual_vector_1.hpp"
#include "virtual_vector_2.hpp"
#include "virtual_vector_3.hpp"

#include <iostream>
#include <memory>
#include <random>
#include <vector>

int main()
{
    #if defined(INLINE) && !defined(VIRTUAL) && !defined(RANDOM)
        const char* label = "InlineVector";
        using VectorAdderType = VectorAdder<Vector, Vector>;
    #elif !defined(INLINE) && !defined(VIRTUAL) && !defined(RANDOM)
        const char* label = "NoInlineVector";
        using VectorAdderType = VectorAdder<Vector, Vector>;
    #elif defined(INLINE) && defined(VIRTUAL) && !defined(RANDOM)
        const char* label = "VirtualVectorInline";
        using VectorAdderType = VectorAdder<VectorBase, VirtualVector_1>;
    #elif !defined(INLINE) && defined(VIRTUAL) && !defined(RANDOM)
        const char* label = "VirtualVectorNoInline";
        using VectorAdderType = VectorAdder<VectorBase, VirtualVector_1>;
    #elif defined(INLINE) && defined(VIRTUAL) && defined(RANDOM)
        const char* label = "VirtualVectorInlineRandomObject";
        using VectorAdderType = VectorAdderRandomObject<VectorBase, VirtualVector_1, VirtualVector_2>;
    #elif !defined(INLINE) && defined(VIRTUAL) && defined(RANDOM)
        const char* label = "VirtualVectorNoInlineRandomObject";
        using VectorAdderType = VectorAdderRandomObject<VectorBase, VirtualVector_1, VirtualVector_2>;
    #else
        #error "Please define one of the following: INLINE, NO_INLINE, VIRTUAL_INLINE, VIRTUAL_NO_INLINE"
    #endif

    auto [avg, max, min] = VectorAdderType().run();

    std::cout << label << "\t" << avg << '\t' << max << '\t' << min << '\n';
}