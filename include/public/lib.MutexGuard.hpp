/**
 * @file      lib.MutexGuard.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020-2021, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_MUTEX_GUARD_HPP_
#define LIB_MUTEX_GUARD_HPP_

#include "lib.Object.hpp"
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
class MutexGuard : public Object<A>
{
    typedef Object<A> Parent;            

public:

    using Parent::isConstructed;

    /**
     * @brief Constructor.
     *
     * @param mutex A mutex for guarding.
     */
    explicit MutexGuard(api::Mutex& mutex) : Parent(),
        mutex_ (mutex){
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
            if( not isConstructed() )
            {
                break;
            }
            if( not mutex_.isConstructed() )
            {
                break;
            }
            res = mutex_.lock();
        } while(false);
        return res;
    }
    
    /**
     * @copydoc eoos::Object::Object(const Object&)
     */
    MutexGuard(const MutexGuard& obj);

    /**
     * @copydoc eoos::Object::operator=(const Object&)
     */
    MutexGuard& operator=(const MutexGuard& obj);
    
    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(const Object&&)
     */       
    MutexGuard(MutexGuard&& obj) noexcept = delete; 
    
    /**
     * @copydoc eoos::Object::operator=(const Object&&)
     */
    MutexGuard& operator=(MutexGuard&& obj) noexcept = delete;
    
    #endif // EOOS_CPP_STANDARD >= 2011

    /**
     * @brief Mutex resource identifier.
     */
    api::Mutex& mutex_;
};

} // namespace lib
} // namespace eoos
#endif // LIB_MUTEX_GUARD_HPP_
