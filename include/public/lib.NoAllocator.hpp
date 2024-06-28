/**
 * @file      lib.NoAllocator.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2023-2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_NOALLOCATOR_HPP_
#define LIB_NOALLOCATOR_HPP_

#include "lib.Types.hpp"

namespace eoos
{
namespace lib
{
 
/**
 * @class NoAllocator
 * @brief No memory allocator.
 */ 
class NoAllocator
{

public:

    /**
     * @brief Does not allocate memory.
     *
     * @return The null pointer.
     */
    static void* allocate(size_t);

    /**
     * @brief Frees allocated memory.
     */
    static void free(void*);

};

inline void* NoAllocator::allocate(size_t)
{
    return NULLPTR;
}

inline void NoAllocator::free(void*)
{
}

} // namespace lib
} // namespace eoos
#endif // LIB_NOALLOCATOR_HPP_
