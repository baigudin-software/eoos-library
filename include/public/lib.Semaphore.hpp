/**
 * @file      lib.Semaphore.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_SEMAPHORE_HPP_
#define LIB_SEMAPHORE_HPP_

#include "lib.NonCopyable.hpp"
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
class Semaphore : public NonCopyable<A>, public api::Semaphore
{
    typedef lib::NonCopyable<A> Parent;

public:

    /**
     * @brief Constructor.
     *
     * @param permits The initial number of permits available.
     */
    Semaphore(const int32_t permits) : Parent(),
        semaphore_ (NULLPTR){
        bool_t const isConstructed = construct(permits);
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
        if( isConstructed() )
        {
            return semaphore_->acquire();
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
        if( isConstructed() )
        {
            semaphore_->release();
        }
    }

protected:

    using Parent::setConstructed;

private:

    /**
     * @brief Constructor.
     *
     * @param permits The initial number of permits available.
     * @return True if object has been constructed successfully.
     */
    bool_t construct(const int32_t permits)
    {
        bool_t const res = isConstructed();
        if( res == false )
        {
            return false;
        }
        semaphore_ = sys::Call::get().createSemaphore(permits);
        return (semaphore_ != NULLPTR) ? semaphore_->isConstructed() : false;
    }

    /**
     * @brief System semaphore interface.
     */
    api::Semaphore* semaphore_;

};

} // namespace lib
} // namespace eoos
#endif // SEMAPHORE_HPP_
