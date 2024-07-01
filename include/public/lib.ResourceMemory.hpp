/**
 * @file      lib.ResourceMemory.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2023-2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_RESOURCEMEMORY_HPP_
#define LIB_RESOURCEMEMORY_HPP_

#include "lib.NonCopyable.hpp"
#include "api.Heap.hpp"
#include "lib.NoAllocator.hpp"
#include "lib.Guard.hpp"

namespace eoos
{
namespace lib
{
 
/**
 * @class ResourceMemory<T,N>
 * @brief Pool resource memory allocator.
 *
 * Primary template implements the pool allocation of resource. 
 * 
 * @tparam T Resource type
 * @tparam N Number of resources
 */
template <typename T, int32_t N>
class ResourceMemory : public NonCopyable<NoAllocator>, public api::Heap
{
    typedef NonCopyable<NoAllocator> Parent;

public:

    /**
     * @brief Constructor.
     *
     * @param guard Atomic access to any resources.
     */
    ResourceMemory(api::Guard& guard);

    /**
     * @brief Destructor.
     */
    virtual ~ResourceMemory();

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
     * @brief Resource allocation guard.
     */
    api::Guard& guard_;

    /**
     * @brief Resource memory pool. 
     */
    bool_t isAllocated_[N]; 

    /**
     * @brief Resource memory pool.
     * 
     * @note Memory is double array of uint64_t type to be align 8.  
     */
    uint64_t memory_[N][(sizeof(T) >> 3) + 1]; 
    
};

/**
 * @class ResourceMemory<T,0>
 * @brief Heap resource memory allocator.
 *
 * Partial specialization of the template implements the heap allocation of resource.
 * 
 * @tparam T Resource type
 */
template <typename T>
class ResourceMemory<T,0> : public NonCopyable<Allocator>, public api::Heap
{
    typedef NonCopyable<Allocator> Parent;

public:

    /**
     * @brief Constructor.
     *
     * @param guard Atomic access to any resources.
     */
    ResourceMemory(api::Guard& guard);

    /**
     * @brief Destructor.
     */
    virtual ~ResourceMemory();

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

template <typename T, int32_t N>
ResourceMemory<T,N>::ResourceMemory(api::Guard& guard)
    : NonCopyable<NoAllocator>()
    , api::Heap()
    , guard_ (guard) {
    bool_t const isConstructed( construct() );
    setConstructed( isConstructed );
}

template <typename T, int32_t N>
ResourceMemory<T,N>::~ResourceMemory()
{
}

template <typename T, int32_t N>
bool_t ResourceMemory<T,N>::isConstructed() const
{
    return Parent::isConstructed();
}

template <typename T, int32_t N>
void* ResourceMemory<T,N>::allocate(size_t size, void* ptr)
{
    static_cast<void>(ptr); // Avoid MISRA-C++:2008 Rule 0–1–3 and AUTOSAR C++14 Rule A0-1-4
    void* addr( NULLPTR );
    if( isConstructed() )
    {
        lib::Guard<NoAllocator> const guard( guard_ );
        if( size == sizeof(T) )
        {
            for(int32_t i(0); i<N; i++)
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

template <typename T, int32_t N>
void ResourceMemory<T,N>::free(void* ptr)
{
    if( isConstructed() && ptr != NULLPTR )
    {
        lib::Guard<NoAllocator> const guard( guard_ );
        for(int32_t i(0); i<N; i++)
        {
            if( (memory_[i] == ptr) && (isAllocated_[i] == true) )
            {
                isAllocated_[i] = false;
                break;
            }
        }
    }
}

template <typename T, int32_t N>
bool_t ResourceMemory<T,N>::construct()
{
    bool_t res( false );
    if( ( isConstructed() )
     && ( guard_.isConstructed() ) ) 
    {    
        for(int32_t i(0); i<N; i++)
        {
            isAllocated_[i] = false;
        }
        res = true;
    }
    return res;
}

template <typename T>
ResourceMemory<T,0>::ResourceMemory(api::Guard&)
    : NonCopyable<Allocator>()
    , api::Heap() {
}

template <typename T>
ResourceMemory<T,0>::~ResourceMemory()
{
}

template <typename T>
bool_t ResourceMemory<T,0>::isConstructed() const
{
    return Parent::isConstructed();
}

template <typename T>
void* ResourceMemory<T,0>::allocate(size_t size, void* ptr)
{
    return Allocator::allocate(size);
}

template <typename T>
void ResourceMemory<T,0>::free(void* ptr)
{
    return Allocator::free(ptr);
}

} // namespace lib
} // namespace eoos
#endif // LIB_RESOURCEMEMORY_HPP_
