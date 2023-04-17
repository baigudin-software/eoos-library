/**
 * @file      lib.Mutex.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2015-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_MUTEX_HPP_
#define LIB_MUTEX_HPP_

#include "lib.NonCopyable.hpp"
#include "api.Mutex.hpp"
#include "sys.Call.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class Mutex<A>
 * @brief Mutex class.
 *
 * @tparam A Heap memory allocator class. 
 */
template <class A = Allocator>
class Mutex : public NonCopyable<A>, public api::Mutex
{
    typedef Mutex<A> Self;
    typedef NonCopyable<A> Parent;

public:

    /**
     * @brief Constructor.
     */
    Mutex() 
        : NonCopyable<A>()
        , api::Mutex()
        , mutex_ (NULLPTR){
        bool_t const isConstructed( construct() );
        setConstructed( isConstructed );
    }

    /**
     * @brief Destructor.
     */
    virtual ~Mutex()
    {
        sys::Call::get().getSystemMutex().remove(mutex_);
    }
    
    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }
    
    /**
     * @copydoc eoos::api::Mutex::tryLock()
     */
    virtual bool_t tryLock()
    {
        bool_t res( false );
        if( Self::isConstructed() )
        {
            res = mutex_->tryLock();
        }
        return res;        
    }        

    /**
     * @copydoc eoos::api::Mutex::lock()
     */
    virtual bool_t lock()
    {
        bool_t res( false );
        if( Self::isConstructed() )
        {
            res = mutex_->lock();
        }
        return res;
    }

    /**
     * @copydoc eoos::api::Mutex::unlock()
     */
    virtual void unlock()
    {
        if( Self::isConstructed() )
        {
            mutex_->unlock();
        }
    }

protected:

    using Parent::setConstructed;

private:

    /**
     * @brief Constructor.
     *
     * @return True if object has been constructed successfully.
     */
    bool_t construct()
    {
        bool_t res( false );
        do
        {   
            if( !Self::isConstructed() )
            {   ///< UT Justified Branch: HW dependency
                break;
            }
            mutex_ = sys::Call::get().getSystemMutex().create();
            if( !Parent::isConstructed(mutex_) )
            {   ///< UT Justified Branch: HW dependency
                break;
            }
            res = true;
        } while(false);
        return res;
    }

    /**
     * @brief System mutex interface.
     */
    api::Mutex* mutex_;

};

} // namespace lib
} // namespace eoos
#endif // LIB_MUTEX_HPP_
