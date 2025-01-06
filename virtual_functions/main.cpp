#include "vector_adder.hpp"
#include "inline_vector.hpp"
#include "no_inline_vector.hpp"
#include "virtual_vector_inline.hpp"
#include "virtual_vector_no_inline.hpp"

#include <iostream>
#include <memory>
#include <random>
#include <vector>

int main()
{
    #ifdef INLINE
        const char* label = "InlineVector";
        using PointerType = InlineVector;
        using ObjectType = InlineVector;
    #elif defined(NO_INLINE)
        const char* label = "NoInlineVector";
        using PointerType = NoInlineVector;
        using ObjectType = NoInlineVector;
    #elif defined(VIRTUAL_INLINE)
        const char* label = "VirtualVectorInline";
        using PointerType = VectorBase;
        using ObjectType = VirtualVectorInline;
    #elif defined(VIRTUAL_NO_INLINE)
        const char* label = "VirtualVectorNoInline";
        using PointerType = VectorBase;
        using ObjectType = VirtualVectorNoInline;
    #else
        #error "Please define one of the following: INLINE, NO_INLINE, VIRTUAL_INLINE, VIRTUAL_NO_INLINE"
    #endif

    auto [avg, max, min] = VectorAdder<PointerType, ObjectType>().run();

    std::cout << label << "\t" << avg << '\t' << max << '\t' << min << '\n';
}