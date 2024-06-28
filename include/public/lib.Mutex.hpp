/**
 * @file      lib.Mutex.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2015-2024, Sergey Baigudin, Baigudin Software
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
    Mutex();

    /**
     * @brief Destructor.
     */
    virtual ~Mutex();
    
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

protected:

    using Parent::setConstructed;

private:

    /**
     * @brief Constructor.
     *
     * @return True if object has been constructed successfully.
     */
    bool_t construct();

    /**
     * @brief System mutex interface.
     */
    api::Mutex* mutex_;

};

template <class A>
Mutex<A>::Mutex() 
    : NonCopyable<A>()
    , api::Mutex()
    , mutex_ (NULLPTR){
    bool_t const isConstructed( construct() );
    setConstructed( isConstructed );
}

template <class A>
Mutex<A>::~Mutex()
{
    if( mutex_ != NULLPTR )
    {
        delete mutex_;
    }
}

template <class A>
bool_t Mutex<A>::isConstructed() const
{
    return Parent::isConstructed();
}

template <class A>
bool_t Mutex<A>::tryLock()
{
    bool_t res( false );
    if( isConstructed() )
    {
        res = mutex_->tryLock();
    }
    return res;        
}        

template <class A>
bool_t Mutex<A>::lock()
{
    bool_t res( false );
    if( isConstructed() )
    {
        res = mutex_->lock();
    }
    return res;
}

template <class A>
bool_t Mutex<A>::unlock()
{
    bool_t res( false );
    if( isConstructed() )
    {
        res = mutex_->unlock();
    }
    return res;
}

template <class A>
bool_t Mutex<A>::construct()
{
    bool_t res( false );
    if( isConstructed() )
    {
        mutex_ = sys::Call::get().getMutexManager().create();
        if( Parent::isConstructed(mutex_) )
        {   
            res = true;
        }
    }
    return res;
}

} // namespace lib
} // namespace eoos
#endif // LIB_MUTEX_HPP_
