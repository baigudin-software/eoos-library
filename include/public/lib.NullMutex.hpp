/**
 * @file      lib.NullMutex.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2025, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_NULLMUTEX_HPP_
#define LIB_NULLMUTEX_HPP_

#include "lib.NonCopyable.hpp"
#include "api.Mutex.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class NullMutex<A>
 * @brief Empty mutex that does no locking at all.
 *
 * @tparam A Heap memory allocator class.
 */
template <class A = Allocator>
class NullMutex : public NonCopyable<A>, public api::Mutex
{
    typedef NonCopyable<A> Parent;

public:

    /**
     * @brief Constructor.
     */
    NullMutex();

    /**
     * @brief Destructor.
     */
    virtual ~NullMutex();

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const;

    /**
     * @copydoc eoos::api::Mutex::tryLock()
     */
    virtual bool_t tryLock();

    /**
     * @copydoc eoos::api::Mutex::lock()
     */
    virtual bool_t lock();

    /**
     * @copydoc eoos::api::Mutex::unlock()
     */
    virtual bool_t unlock();

};

template <class A>
NullMutex<A>::NullMutex()
    : NonCopyable<A>()
    , api::Mutex() {
}

template <class A>
NullMutex<A>::~NullMutex()
{
}

template <class A>
bool_t NullMutex<A>::isConstructed() const
{
    return Parent::isConstructed();
}

template <class A>
bool_t NullMutex<A>::tryLock()
{
    bool_t res( false );
    if( isConstructed() )
    {
        res = true;
    }
    return res;
}

template <class A>
bool_t NullMutex<A>::lock()
{
    bool_t res( false );
    if( isConstructed() )
    {
        res = true;
    }
    return res;
}

template <class A>
bool_t NullMutex<A>::unlock()
{
    bool_t res( false );
    if( isConstructed() )
    {
        res = true;
    }
    return res;
}

} // namespace lib
} // namespace eoos
#endif // LIB_MUTEX_HPP_
