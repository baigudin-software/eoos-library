/**
 * @file      ObjectAllocator.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2021-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_OBJECTALLOCATOR_HPP_
#define LIB_OBJECTALLOCATOR_HPP_

#include "lib.Allocator.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class ObjectAllocator<A>
 * @brief Object memory allocator.
 *
 * @tparam A Heap memory allocator class.
 */
template <class A = Allocator>
class ObjectAllocator
{

public:

    #ifdef EOOS_ENABLE_DYNAMIC_HEAP_MEMORY

    /**
     * @brief Operator new.
     *
     * @param size A number of bytes to allocate.
     * @return Allocated memory address or a null pointer.
     */
    static void* operator new(size_t const size) EOOS_KEYWORD_NOEXCEPT 
    {
        return A::allocate(size);
    }

    /**
     * @brief Operator delete.
     *
     * @param ptr An address of allocated memory block or a null pointer.
     */
    static void operator delete(void* const ptr)
    {
        A::free(ptr);
    }

    /**
     * @brief Operator new.
     *
     * @param ptr A pointer to reserved memory area.
     * @return The given pointer.
     */
    static void* operator new(size_t, void* const ptr) EOOS_KEYWORD_NOEXCEPT
    {
        return ptr;
    }
    
    /**
     * @brief Operator delete.
     */
    static void operator delete(void*, void*)
    {
    }

    #endif // EOOS_ENABLE_DYNAMIC_HEAP_MEMORY
    
protected:

    /**
     * @brief Destructor.
     *
     * @note It's prohibited to cast to this class to delete any child classes.
     */
    ~ObjectAllocator()
    {
    }    

};

} // namespace lib
} // namespace eoos

#endif // OBJECTALLOCATOR_HPP_
