/**
 * @file      lib.Semaphore.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2023, Sergey Baigudin, Baigudin Software
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
    explicit Semaphore(int32_t const permits) 
        : NonCopyable<A>()
        , api::Semaphore()
        , semaphore_(NULLPTR) {
        bool_t const isConstructed( construct(permits) );
        setConstructed( isConstructed );
    }

    /**
     * @brief Destructor.
     */
    virtual ~Semaphore()
    {
        if( semaphore_ != NULLPTR )
        {
            delete semaphore_;
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
     * @copydoc eoos::api::Semaphore::acquire()
     */
    virtual bool_t acquire()
    {
        bool_t res( false );
        if( isConstructed() )
        {
            res = semaphore_->acquire();
        }
        return res;
    }

    /**
     * @copydoc eoos::api::Semaphore::release()
     */
    virtual bool_t release()
    {
        bool_t res( false );        
        if( isConstructed() )
        {
            res = semaphore_->release();
        }
        return res;
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
    bool_t construct(int32_t const permits)
    {
        bool_t res( false );
        do
        {   
            if( !isConstructed() )
            {   ///< UT Justified Branch: HW dependency
                break;
            }
            semaphore_ = sys::Call::get().getSemaphoreManager().create(permits);
            if( !Parent::isConstructed(semaphore_) )
            {   ///< UT Justified Branch: HW dependency
                break;
            }
            res = true;
        } while(false);
        return res;
    }

    /**
     * @brief System semaphore interface.
     */
    api::Semaphore* semaphore_;

};

} // namespace lib
} // namespace eoos
#endif // SEMAPHORE_HPP_
