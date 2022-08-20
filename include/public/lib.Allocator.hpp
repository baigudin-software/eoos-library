/**
 * @file      lib.Allocator.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ALLOCATOR_HPP_
#define LIB_ALLOCATOR_HPP_

#include "lib.Types.hpp"
#include "sys.Call.hpp"

namespace eoos
{
namespace lib
{
 
/**
 * @class Allocator
 * @brief Memory allocator.
 */ 
class Allocator
{

public:

    /**
     * @brief Allocates memory.
     *
     * @param size Number of bytes to allocate.
     * @return Allocated memory address or a null pointer.
     */
    static void* allocate(size_t size)
    {
        return sys::Call::get().getHeap().allocate(size, NULLPTR);
    }

    /**
     * @brief Frees allocated memory.
     *
     * @param ptr Address of allocated memory block or a null pointer.
     */
    static void free(void* ptr)
    {
        return sys::Call::get().getHeap().free(ptr);    
    }
};

} // namespace lib
} // namespace eoos

#endif // LIB_ALLOCATOR_HPP_
