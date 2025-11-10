/**
 * @file      lib.MemoryPool.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2023-2025, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_MEMORYPOOL_HPP_
#define LIB_MEMORYPOOL_HPP_

#include "lib.NonCopyable.hpp"
#include "api.Heap.hpp"
#include "lib.NoAllocator.hpp"
#include "lib.NullMutex.hpp"
#include "lib.Guard.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class MemoryPool<T,L,A>
 * @brief Allocator of fix size memory block from pool.
 *
 * Primary template implements the pool allocation of resource.
 *
 * @tparam T Element type allocating from pool
 * @tparam L Maximum number of pool elements, or 0 for dynamic allocation.
 * @tparam A Heap memory allocator class.
 */
template <typename T, int32_t L, class A = Allocator>
class MemoryPool : public NonCopyable<A>, public api::Heap
{
    typedef NonCopyable<A> Parent;

public:

    /**
     * @brief Constructor.
     */
    MemoryPool();

    /**
     * @brief Constructor.
     *
     * @param guard Atomic access to any resources.
     */
    MemoryPool(api::Guard& guard);

    /**
     * @brief Destructor.
     */
    virtual ~MemoryPool();

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const;

    /**
     * @copydoc eoos::api::Heap::allocate(size_t,void*)
     */
    virtual void* allocate(size_t size, void* ptr);

    /**
     * @copydoc eoos::api::Heap::free(void*)
     */
    virtual void free(void* ptr);

protected:

    using Parent::setConstructed;

private:

    /**
     * @brief Constructs this object.
     *
     * @return True if object has been constructed successfully.
     */
    bool_t construct();

    /**
     * @brief Resource memory pool.
     *
     * @note Memory is double array of uint64_t type to be align 8.
     */
    uint64_t memory_[L][(sizeof(T) >> 3) + 1];

    /**
     * @brief Resource memory pool.
     */
    bool_t isAllocated_[L];

    /**
     * @brief Empty mutex.
     */
    NullMutex<> mutex_;

    /**
     * @brief Resource allocation guard.
     */
    api::Guard& guard_;

};

/**
 * @class MemoryPool<T,0,A>
 * @brief Allocator of fix size memory block from heap
 *
 * Partial specialization of the template implements the heap allocation of resource.
 *
 * @tparam T Element type allocating from pool
 * @tparam A Heap memory allocator class for this class and element allocation.
 */
template <typename T, class A>
class MemoryPool<T,0,A> : public NonCopyable<A>, public api::Heap
{
    typedef NonCopyable<A> Parent;

public:

    /**
     * @brief Constructor.
     */
    MemoryPool();

    /**
     * @brief Constructor.
     *
     * @param guard Atomic access to any resources.
     */
    MemoryPool(api::Guard& guard);

    /**
     * @brief Destructor.
     */
    virtual ~MemoryPool();

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const;

    /**
     * @copydoc eoos::api::Heap::allocate(size_t,void*)
     */
    virtual void* allocate(size_t size, void* ptr);

    /**
     * @copydoc eoos::api::Heap::free(void*)
     */
    virtual void free(void* ptr);

protected:

    using Parent::setConstructed;

};

template <typename T, int32_t L, class A>
MemoryPool<T,L,A>::MemoryPool()
    : NonCopyable<A>()
    , api::Heap()
    , mutex_()
    , guard_(mutex_) {
    bool_t const isConstructed( construct() );
    setConstructed( isConstructed );
}

template <typename T, int32_t L, class A>
MemoryPool<T,L,A>::MemoryPool(api::Guard& guard)
    : NonCopyable<A>()
    , api::Heap()
    , mutex_()
    , guard_(guard) {
    bool_t const isConstructed( construct() );
    setConstructed( isConstructed );
}

template <typename T, int32_t L, class A>
MemoryPool<T,L,A>::~MemoryPool()
{
}

template <typename T, int32_t L, class A>
bool_t MemoryPool<T,L,A>::isConstructed() const
{
    return Parent::isConstructed();
}

template <typename T, int32_t L, class A>
void* MemoryPool<T,L,A>::allocate(size_t size, void* ptr)
{
    static_cast<void>(ptr); // Avoid MISRA-C++:2008 Rule 0–1–3 and AUTOSAR C++14 Rule A0-1-4
    void* addr( NULLPTR );
    if( isConstructed() )
    {
        lib::Guard<A> const guard( guard_ );
        if( size == sizeof(T) )
        {
            // @todo Declare 'i' as class variable to point to the first empty element.
            for(int32_t i(0); i<L; i++)
            {
                if( isAllocated_[i] == false )
                {
                    uint64_t* const memory( memory_[i] );
                    uintptr_t const address( reinterpret_cast<uintptr_t>(memory) );
                    if( ( address & 0x7 ) == 0 )
                    {
                        isAllocated_[i] = true;
                        addr = memory;
                        break;
                    }
                }
            }
        }
    }
    return addr;
}

template <typename T, int32_t L, class A>
void MemoryPool<T,L,A>::free(void* ptr)
{
    if( isConstructed() && (ptr != NULLPTR) )
    {
        lib::Guard<A> const guard( guard_ );
        for(int32_t i(0); i<L; i++)
        {
            if( (memory_[i] == ptr) && (isAllocated_[i] == true) )
            {
                isAllocated_[i] = false;
                break;
            }
        }
    }
}

template <typename T, int32_t L, class A>
bool_t MemoryPool<T,L,A>::construct()
{
    bool_t res( false );
    if( ( isConstructed() )
     && ( guard_.isConstructed() ) )
    {
        for(int32_t i(0); i<L; i++)
        {
            isAllocated_[i] = false;
        }
        res = true;
    }
    return res;
}

template <typename T, class A>
MemoryPool<T,0,A>::MemoryPool()
    : NonCopyable<A>()
    , api::Heap() {
}

template <typename T, class A>
MemoryPool<T,0,A>::MemoryPool(api::Guard&)
    : NonCopyable<A>()
    , api::Heap() {
}

template <typename T, class A>
MemoryPool<T,0,A>::~MemoryPool()
{
}

template <typename T, class A>
bool_t MemoryPool<T,0,A>::isConstructed() const
{
    return Parent::isConstructed();
}

template <typename T, class A>
void* MemoryPool<T,0,A>::allocate(size_t size, void* ptr)
{
    return A::allocate(size);
}

template <typename T, class A>
void MemoryPool<T,0,A>::free(void* ptr)
{
    return A::free(ptr);
}

} // namespace lib
} // namespace eoos
#endif // LIB_MEMORYPOOL_HPP_
