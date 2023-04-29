/**
 * @file      lib.Mutex.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2015-2023, Sergey Baigudin, Baigudin Software
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
        api::System& system = sys::Call::get();
        if( system.hasMutexManager() )
        {
            system.getMutexManager().remove(mutex_);
        }
    }
    
    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }
    
    /**
     * @copydoc eoos::api::Resource::getId()
     */
    virtual int32_t getId() const
    {
        int32_t id( -1 );
        if( isConstructed() )
        {
            id = mutex_->getId();
        }
        return id;
    }

    /**
     * @copydoc eoos::api::Mutex::tryLock()
     */
    virtual bool_t tryLock()
    {
        bool_t res( false );
        if( isConstructed() )
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
        if( isConstructed() )
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
        if( isConstructed() )
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
            if( !isConstructed() )
            {   ///< UT Justified Branch: HW dependency
                break;
            }
            api::System& system = sys::Call::get();
            if( !system.hasMutexManager() )
            {
                break;
            }
            mutex_ = system.getMutexManager().create();
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
