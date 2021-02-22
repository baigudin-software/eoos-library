/**
 * @brief Mutex.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2015-2021, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_MUTEX_HPP_
#define LIB_MUTEX_HPP_

#include "lib.Object.hpp"
#include "api.Mutex.hpp"
#include "sys.Call.hpp"

namespace eoos
{
namespace lib
{

/**
 * @brief Mutex class.
 *
 * @tparam A heap memory allocator class. 
 */
template <class A = Allocator>
class Mutex : public Object<A>, public api::Mutex
{
    typedef Mutex Self;
    typedef Object<A> Parent;

public:

    /**
     * @brief Constructor.
     */
    Mutex() : Parent(),
        mutex_ (NULLPTR){
        bool_t const isConstructed = construct();
        setConstructed( isConstructed );
    }

    /**
     * @brief Destructor.
     */
    virtual ~Mutex()
    {
        delete mutex_;
    }
    
    /**
     * @brief Tests if this object has been constructed.
     *
     * @return true if object has been constructed successfully.
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }
    
    /**
     * @brief Tries to locks this mutex.
     *
     * @return true if this mutex is locked successfully, or false if other thread locked on this mutex.
     */
    virtual bool_t tryLock()
    {
        bool_t res = false;
        if( Self::isConstructed() )
        {
            res = mutex_->tryLock();
        }
        return res;        
    }        

    /**
     * @brief Locks the mutex.
     *
     * @return true if the mutex is lock successfully.
     */
    virtual bool_t lock()
    {
        bool_t res = false;
        if( Self::isConstructed() )
        {
            res = mutex_->lock();
        }
        return res;
    }

    /**
     * @brief Unlocks the mutex.
     */
    virtual void unlock()
    {
        if( Self::isConstructed() )
        {
            mutex_->unlock();
        }
    }

private:

    /**
     * @brief Constructor.
     *
     * @return true if object has been constructed successfully.
     */
    bool_t construct()
    {
        bool_t res = false;
        do
        {   
            if( not Self::isConstructed() )
            {
                break;
            }
            mutex_ = sys::Call::get().createMutex();
            if( not Parent::isConstructed(mutex_) )
            {
                break;
            }
            res = true;
        } while(false);
        return res;
    }

    /**
     * @brief Copy constructor.
     *
     * @param obj - reference to source object.
     */
    Mutex(const Mutex& obj);

    /**
     * @brief Assignment operator.
     *
     * @param obj - reference to source object.
     * @return reference to this object.
     */
    Mutex& operator=(const Mutex& obj);
    
    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @brief Move constructor.
     *
     * @param obj Right reference to a source object.     
     */       
    Mutex(Mutex&& obj) noexcept = delete; 
    
    /**
     * @brief Move assignment operator.
     *
     * @param obj Right reference to a source object.
     * @return reference to this object.
     */
    Mutex& operator=(Mutex&& obj) noexcept = delete;
    
    #endif // EOOS_CPP_STANDARD >= 2011    

    /**
     * @brief System mutex interface.
     */
    api::Mutex* mutex_;

};

} // namespace lib
} // namespace eoos
#endif // LIB_MUTEX_HPP_
