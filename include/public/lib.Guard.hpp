/**
 * @file      lib.Guard.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020-2023, Sergey Baigudin, Baigudin Software
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
    Guard(api::Guard& guard) 
        : NonCopyable<A>()
        , guard_( guard ){
        bool_t const isConstructed( construct() );
        setConstructed( isConstructed );                    
    }
    
    /**        
     * @brief Destructor.
     */
    virtual ~Guard()
    {
        if( isConstructed() )
        {
            static_cast<void>( guard_.unlock() );
        }
    }
    
protected:

    using Parent::setConstructed;    
        
private:

    /**        
     * @brief Constructs this object.
     *
     * @return True if this object has been constructed successfully.
     */
    bool_t construct()
    {
        bool_t res( false );
        if( isConstructed() )
        {
            res = guard_.lock();
        }
        return res;
    }
    
    /**
     * @brief Guard resource identifier.
     */
    api::Guard& guard_;
};

} // namespace lib
} // namespace eoos
#endif // LIB_GUARD_HPP_
