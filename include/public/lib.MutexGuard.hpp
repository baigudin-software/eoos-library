/**
 * @brief Guard of mutex.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020, Sergey Baigudin, Baigudin Software
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
 * @brief Mutex class.
 *
 * @tparam A heap memory allocator class.
 */    
template <class A = Allocator>
class MutexGuard : public Object<A>
{
    typedef Object<A> Parent;            

public:

    /**
     * @brief Constructor.
     *
     * @param mutex - a mutex for guarding.
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
        
private:

    /**        
     * @brief Constructs this object.
     *
     * @returns true if this object has been constructed successfully.
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
     * @brief Copy constructor.
     */
    MutexGuard(const MutexGuard&);

    /**        
     * @brief Copy assignment operator.
     */
    MutexGuard& operator=(const MutexGuard&);

    /**
     * @brief Mutex resource identifier.
     */
    api::Mutex& mutex_;
};


} // namespace lib
} // namespace eoos
#endif // LIB_MUTEX_GUARD_HPP_
