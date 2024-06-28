/**
 * @file      ObjectAllocator.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2021-2024, Sergey Baigudin, Baigudin Software
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

    /**
     * @brief Operator new.
     *
     * @param size A number of bytes to allocate.
     * @return Allocated memory address or a null pointer.
     */
    static void* operator new(size_t const size) EOOS_KEYWORD_NOEXCEPT;

    /**
     * @brief Operator delete.
     *
     * @param ptr An address of allocated memory block or a null pointer.
     */
    static void operator delete(void* const ptr);

    /**
     * @brief Operator new.
     *
     * @param ptr A pointer to reserved memory area.
     * @return The given pointer.
     */
    static void* operator new(size_t, void* const ptr) EOOS_KEYWORD_NOEXCEPT;
    
    /**
     * @brief Operator delete.
     */
    static void operator delete(void*, void*);
    
protected:

    /**
     * @brief Destructor.
     *
     * @note It's prohibited to cast to this class to delete any child classes.
     */
    ~ObjectAllocator();

};

template <class A>
void* ObjectAllocator<A>::operator new(size_t const size) EOOS_KEYWORD_NOEXCEPT 
{
    return A::allocate(size);
}

template <class A>
void ObjectAllocator<A>::operator delete(void* const ptr)
{
    A::free(ptr);
}

template <class A>
void* ObjectAllocator<A>::operator new(size_t, void* const ptr) EOOS_KEYWORD_NOEXCEPT
{
    return ptr;
}

template <class A>
void ObjectAllocator<A>::operator delete(void*, void*)
{   ///< UT Justified Branch: Language dependency
}

template <class A>
ObjectAllocator<A>::~ObjectAllocator()
{
}

} // namespace lib
} // namespace eoos

#endif // OBJECTALLOCATOR_HPP_
