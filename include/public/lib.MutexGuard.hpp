/**
 * @file      lib.MutexGuard.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020-2021, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_MUTEXGUARD_HPP_
#define LIB_MUTEXGUARD_HPP_

#include "lib.NonCopyable.hpp"
#include "lib.Mutex.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class MutexGuard<A>
 * @brief Guard of mutex.
 *
 * @tparam A heap memory allocator class.
 */    
template <class A = Allocator>
class MutexGuard : public NonCopyable<A>
{
    typedef NonCopyable<A> Parent;            

public:

    using Parent::isConstructed;

    /**
     * @brief Constructor.
     *
     * @param mutex A mutex for guarding.
     */
    explicit MutexGuard(api::Mutex& mutex) 
        : NonCopyable<A>()
        , mutex_ (mutex){
        bool_t const isConstructed = construct();
        setConstructed( isConstructed );                    
    }
    
    /**        
     * @brief Destructor.
     */
    virtual ~MutexGuard()
    {
        if( isConstructed() )
        {
            mutex_.unlock();
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
        bool_t res = false;
        do
        {
            if( !isConstructed() )
            {
                break;
            }
            if( !mutex_.isConstructed() )
            {
                break;
            }
            res = mutex_.lock();
        } while(false);
        return res;
    }

    /**
     * @brief Mutex resource identifier.
     */
    api::Mutex& mutex_;
};

} // namespace lib
} // namespace eoos
#endif // LIB_MUTEXGUARD_HPP_
