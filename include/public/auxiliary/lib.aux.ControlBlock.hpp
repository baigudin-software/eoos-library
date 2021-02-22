/**
 * @brief Shared pointer control block class.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_AUX_CONTROL_BLOCK_HPP_
#define LIB_AUX_CONTROL_BLOCK_HPP_

#include "lib.Object.hpp"
#include "lib.MutexGuard.hpp"

namespace eoos
{
namespace lib
{
namespace aux
{

#ifdef EOOS_NO_STRICT_MISRA_RULES

/**
 * @brief Primary template implementation of shared pointer control block class.
 *
 * @tparam T Data type of owning the object.
 * @tparam D Deleter type for owning the object.  
 * @tparam A Heap memory allocator class.
 * @tparam M Mutex to protect the control block inherited from @ref api::Mutex.
 *
 * @note This class is implemented as an auxiliry class for SharedPtr
 * and is tested for construction before usage. Therefore, some checks
 * are skipped in public interface to speedup performence. 
 */
template <typename T, class D, class A = Allocator, class M = Mutex<A> > 
class ControlBlock : public Object<A>
{
    typedef Object<A> Parent;    
    
public:
    
    /**
     * @brief Constructor.
     *
     * @param pointer A pointer to get ownership.
     */
    explicit ControlBlock(T* const pointer) : Parent(),
        pointer_ (pointer),
        counter_ (1),
        mutex_   (){
        bool_t const isConstructed = construct();
        setConstructed(isConstructed);
    }
    
    /**
     * @brief Destructor.
     */
    virtual ~ControlBlock()
    {
        D::free(pointer_);
    }    
    
    /**
     * @brief Increases the counter on one.
     */
    void increase()
    {
        MutexGuard<A> const guard(mutex_);
        ++counter_;
    }
    
    /**
     * @brief Decreases the counter on one.
     *
     * @return a value of the counter after decreasing.
     */        
    int32_t decrease()
    {
        MutexGuard<A> const guard(mutex_);        
        return --counter_;
    }
    
    /**
     * @brief Returns the counter.
     *
     * @return a value of the counter.
     */        
    int32_t getCounter() const
    {    
        return counter_;
    }
    
    /**
     * @brief Returns the counter.
     *
     * @return a value of the counter.
     */        
    T* getPointer() const
    {
        return pointer_;
    }    

private:

    /**
     * @brief Constructs this object.
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
            res = true;
        } while(false);
        return res;
    }

    /**
     * @brief Copy constructor.
     *
     * @param obj Reference to a source object.
     */
    ControlBlock(const ControlBlock& obj);
    
    /**
     * @brief Copy assignment operator.
     *
     * @param obj   Reference to a source object.
     * @return reference to this object.
     */       
    ControlBlock& operator=(const ControlBlock& obj);

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @brief Move constructor.
     *
     * @param obj Right reference to a source object.     
     */       
    ControlBlock(ControlBlock&& obj) noexcept = delete; 
    
    /**
     * @brief Move assignment operator.
     *
     * @param obj Right reference to a source object.
     * @return reference to this object.
     */
    ControlBlock& operator=(ControlBlock&& obj) noexcept = delete;
    
    #endif // EOOS_CPP_STANDARD >= 2011

    /**
     * @brief An owned pointer.
     */
    T* pointer_;
    
    /**
     * @brief Counter of copies of the shared objects.
     */
    int32_t counter_;

    /**
     * @brief Mutex to protect the counter.
     */    
    M mutex_;
};

#endif // EOOS_NO_STRICT_MISRA_RULES

} // namespace aux
} // namespace lib
} // namespace eoos
#endif // LIB_AUX_CONTROL_BLOCK_HPP_
