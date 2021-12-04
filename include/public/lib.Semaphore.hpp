/**
 * @file      lib.Semaphore.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2021, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_SEMAPHORE_HPP_
#define LIB_SEMAPHORE_HPP_

#include "lib.Object.hpp"
#include "api.Semaphore.hpp"
#include "sys.Call.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class Semaphore<A>
 * @brief Semaphore class.
 *
 * @tparam A Heap memory allocator class. 
 */
template <class A = Allocator>
class Semaphore : public Object<A>, public api::Semaphore
{
    typedef Semaphore Self;
    typedef lib::Object<A> Parent;

public:

    /**
     * @brief Constructor.
     *
     * @param permits The initial number of permits available.
     */
    Semaphore(const int32_t permits) : Parent(),
        semaphore_ (NULLPTR){
        bool_t const isConstructed = construct(permits, NULLPTR);
        setConstructed( isConstructed );
    }

    /**
     * @brief Constructor.
     *
     * @param permits The initial number of permits available.
     * @param isFair  True if this semaphore will guarantee FIFO granting of permits under contention.
     */
    Semaphore(const int32_t permits, const bool_t isFair) : Parent(),
        semaphore_ (NULLPTR){
        bool_t const isConstructed = construct(permits, &isFair);
        setConstructed( isConstructed );
    }

    /**
     * @brief Destructor.
     */
    virtual ~Semaphore()
    {
        delete semaphore_;
    }

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }

    /**
     * @copydoc eoos::api::Semaphore::acquire()
     */
    virtual bool_t acquire()
    {
        if( Self::isConstructed() )
        {
            return semaphore_->acquire();
        }
        else
        {
            return false;
        }
    }

    /**
     * @copydoc eoos::api::Semaphore::acquire(int32_t)
     */
    virtual bool_t acquire(int32_t const permits)
    {
        if( Self::isConstructed() )
        {
            return semaphore_->acquire(permits);
        }
        else
        {
            return false;
        }
    }

    /**
     * @copydoc eoos::api::Semaphore::release()
     */
    virtual void release()
    {
        if( Self::isConstructed() )
        {
            semaphore_->release();
        }
    }

    /**
     * @copydoc eoos::api::Semaphore::release(int32_t)
     */
    virtual void release(int32_t const permits)
    {
        if( Self::isConstructed() )
        {
            semaphore_->release(permits);
        }
    }

    /**
     * @copydoc eoos::api::Semaphore::isFair()
     */
    virtual bool_t isFair() const
    {
        if( Self::isConstructed() )
        {
            return semaphore_->isFair();
        }
        else
        {
            return false;
        }
    }

protected:

    using Parent::setConstructed;

private:

    /**
     * @brief Constructor.
     *
     * @param permits The initial number of permits available.
     * @param isFair  True if this semaphore will guarantee FIFO granting of permits under contention.
     * @return True if object has been constructed successfully.
     */
    bool_t construct(const int32_t permits, const bool_t* const isFair)
    {
        bool_t res = Self::isConstructed();
        if( res == true )
        {
            return false;
        }
        if( isFair == NULLPTR )
        {
            semaphore_ = sys::Call::get().createSemaphore(permits, false);
        }
        else
        {
            semaphore_ = sys::Call::get().createSemaphore(permits, *isFair);
        }
        return semaphore_ != NULLPTR ? semaphore_->isConstructed() : false;
    }

    /**
     * @copydoc eoos::Object::Object(const Object&)
     */
    Semaphore(const Semaphore& obj);

    /**
     * @copydoc eoos::Object::operator=(const Object&)
     */
    Semaphore& operator=(const Semaphore& obj);

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(const Object&&)
     */       
    Semaphore(Semaphore&& obj) noexcept = delete; 
    
    /**
     * @copydoc eoos::Object::operator=(const Object&&)
     */
    Semaphore& operator=(Semaphore&& obj) noexcept = delete;
    
    #endif // EOOS_CPP_STANDARD >= 2011

    /**
     * @brief System semaphore interface.
     */
    api::Semaphore* semaphore_;

};

} // namespace lib
} // namespace eoos
#endif // SEMAPHORE_HPP_
