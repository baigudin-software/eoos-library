/**
 * @brief Semaphore.
 *
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
 * @brief Semaphore class.
 *
 * @tparam A heap memory allocator class. 
 */
template <class A = Allocator>
class Semaphore : public Object<A>, public api::Semaphore
{
    typedef Semaphore Self;
    typedef Object<A> Parent;

public:

    /**
     * @brief Constructor.
     *
     * @param permits - the initial number of permits available.
     */
    Semaphore(const int32_t permits) : Parent(),
        semaphore_ (NULLPTR){
        bool_t const isConstructed = construct(permits, NULLPTR);
        setConstructed( isConstructed );
    }

    /**
     * @brief Constructor.
     *
     * @param permits - the initial number of permits available.
     * @param isFair  - true if this semaphore will guarantee FIFO granting of permits under contention.
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
     * @brief Tests if this object has been constructed.
     *
     * @return true if object has been constructed successfully.
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }

    /**
     * @brief Acquires one permit from this semaphore.
     *
     * @return true if the semaphore is acquired successfully.
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
     * @brief Acquires the given number of permits from this semaphore.
     *
     * @param permits - the number of permits to acquire.
     * @return true if the semaphore is acquired successfully.
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
     * @brief Releases one permit.
     */
    virtual void release()
    {
        if( Self::isConstructed() )
        {
            semaphore_->release();
        }
    }

    /**
     * @brief Releases the given number of permits.
     *
     * @param permits - the number of permits to release.
     */
    virtual void release(int32_t const permits)
    {
        if( Self::isConstructed() )
        {
            semaphore_->release(permits);
        }
    }

    /**
     * @brief Tests if this semaphore is fair.
     *
     * @return true if this semaphore has fairness set true.
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

    /**
     * @brief Tests if this resource is blocked.
     *
     * @return true if this resource is blocked.
     */
    virtual bool_t isBlocked() const
    {
        if( Self::isConstructed() )
        {
            return semaphore_->isBlocked();
        }
        else
        {
            return false;
        }
    }

private:

    /**
     * @brief Constructor.
     *
     * @param permits - the initial number of permits available.
     * @param isFair  - true if this semaphore will guarantee FIFO granting of permits under contention.
     * @return true if object has been constructed successfully.
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
     * @brief Copy constructor.
     *
     * @param obj - reference to source object.
     */
    Semaphore(const Semaphore& obj);

    /**
     * @brief Assignment operator.
     *
     * @param obj - reference to source object.
     * @return reference to this object.
     */
    Semaphore& operator=(const Semaphore& obj);

    /**
     * @brief System semaphore interface.
     */
    api::Semaphore* semaphore_;

};

} // namespace lib
} // namespace eoos
#endif // SEMAPHORE_HPP_
