/**
 * @file      lib.Semaphore.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2024, Sergey Baigudin, Baigudin Software
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
    explicit Semaphore(int32_t const permits);

    /**
     * @brief Destructor.
     */
    virtual ~Semaphore();

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const;

    /**
     * @copydoc eoos::api::Semaphore::acquire()
     */
    virtual bool_t acquire();

    /**
     * @copydoc eoos::api::Semaphore::release()
     */
    virtual bool_t release();

protected:

    using Parent::setConstructed;

private:

    /**
     * @brief Constructor.
     *
     * @param permits The initial number of permits available.
     * @return True if object has been constructed successfully.
     */
    bool_t construct(int32_t const permits);

    /**
     * @brief System semaphore interface.
     */
    api::Semaphore* semaphore_;

};

template <class A>
Semaphore<A>::Semaphore(int32_t const permits) 
    : NonCopyable<A>()
    , api::Semaphore()
    , semaphore_(NULLPTR) {
    bool_t const isConstructed( construct(permits) );
    setConstructed( isConstructed );
}

template <class A>
Semaphore<A>::~Semaphore()
{
    if( semaphore_ != NULLPTR )
    {
        delete semaphore_;
    }
}

template <class A>
bool_t Semaphore<A>::isConstructed() const
{
    return Parent::isConstructed();
}

template <class A>
bool_t Semaphore<A>::acquire()
{
    bool_t res( false );
    if( isConstructed() )
    {
        res = semaphore_->acquire();
    }
    return res;
}

template <class A>
bool_t Semaphore<A>::release()
{
    bool_t res( false );        
    if( isConstructed() )
    {
        res = semaphore_->release();
    }
    return res;
}

template <class A>
bool_t Semaphore<A>::construct(int32_t const permits)
{
    bool_t res( false );
    if( isConstructed() )
    {
        semaphore_ = sys::Call::get().getSemaphoreManager().create(permits);
        if( Parent::isConstructed(semaphore_) )
        {
            res = true;
        }
    }
    return res;
}

} // namespace lib
} // namespace eoos
#endif // SEMAPHORE_HPP_
