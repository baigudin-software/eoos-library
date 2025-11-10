/**
 * @file      lib.Guard.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020-2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_GUARD_HPP_
#define LIB_GUARD_HPP_

#include "lib.NonCopyable.hpp"
#include "api.Guard.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class Guard<A>
 * @brief Guard of RAII implementation.
 *
 * @tparam A heap memory allocator class.
 */
template <class A = Allocator>
class Guard : public NonCopyable<A>
{
    typedef NonCopyable<A> Parent;

public:

    using Parent::isConstructed;

    /**
     * @brief Constructor.
     *
     * @param guard A guard for guarding.
     */
    Guard(api::Guard& guard);

    /**
     * @brief Destructor.
     */
    virtual ~Guard();

private:

    /**
     * @brief Guard resource identifier.
     */
    api::Guard& guard_;

    /**
     * @brief Guard resource identifier.
     */
    bool_t isLocked_;

};

template <class A>
Guard<A>::Guard(api::Guard& guard)
    : NonCopyable<A>()
    , guard_( guard )
    , isLocked_( false ){
    if( isConstructed() )
    {
        isLocked_ = guard_.lock();
    }
}

template <class A>
Guard<A>::~Guard()
{
    if( isLocked_ )
    {
        static_cast<void>( guard_.unlock() );
    }
}

} // namespace lib
} // namespace eoos
#endif // LIB_GUARD_HPP_
