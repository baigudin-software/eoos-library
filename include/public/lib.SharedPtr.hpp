/**
 * @brief Shared pointer class.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_SHARED_PTR_HPP_
#define LIB_SHARED_PTR_HPP_

#include "lib.Object.hpp"

namespace eoos
{
namespace lib
{

#ifdef EOOS_NO_STRICT_MISRA_RULES

/**
 * @brief Deleter of shared pointers allocate with new operator.
 */
template <typename T>
class SharedPtrDeleter
{
public:
	static void free(T* const ptr)
	{
		delete ptr;
	}
};

/**
 * @brief Deleter of shared pointers allocate with new [] operator.
 */
template <typename T>
class SharedPtrDeleterArray
{
public:
	static void free(T* const ptr)
	{
		delete [] ptr;
	}
};
    
/**
 * @brief Primary template implementation.
 *
 * @param T Data type of shared pointer.
 * @param D Deleter type of shared pointer.  
 * @param A Heap memory allocator class.
 */
template <typename T, class D = SharedPtrDeleter<T>, class A = Allocator>
class SharedPtr : public Object<A>
{
    typedef SharedPtr<T,A>  Self;
    typedef Object<A> Parent;

public:

    /**
     * @brief Constructor.
     *
     * @param pointer A pointer to get ownership.
     */
    explicit SharedPtr(T* const pointer) : Parent(),
        pointer_ (pointer),
        counter_ (NULLPTR){
        bool_t const isConstructed = construct();
        setConstructed( isConstructed );    
    }

    /**
     * @brief Destructor.
     */
    virtual ~SharedPtr()
    {
        if(isConstructed())
        {
            int32_t counter = counter_->decrease();
            if(counter == 0)
            {
                delete counter_;
                D::free(pointer_);
                counter_ = NULLPTR;
                pointer_ = NULLPTR;            
            }
        }
    }
    
    /**
     * @brief Copy constructor.
     *
     * @param obj Reference to a source object.
     */
    SharedPtr(const SharedPtr& obj)
    {
    }
    
    
    /**
     * @brief Copy assignment operator.
     *
     * @param obj   Reference to a source object.
     * @return reference to this object.
     */       
    SharedPtr& operator=(const SharedPtr& obj)
    {
        return *this;
    }    

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @brief Move constructor.
     *
     * @param obj Right reference to a source object.     
     */       
    SharedPtr(SharedPtr&& obj) :
    {
    }   
    
    /**
     * @brief Move assignment operator.
     *
     * @param obj Right reference to a source object.
     * @return reference to this object.
     */
    SharedPtr& operator=(SharedPtr&& obj)
    {
        return *this;
    }        
    
    #endif // EOOS_CPP_STANDARD >= 2011
    
private:

    /**        
     * @brief Constructs this object.
     *
     * @return true if this object has been constructed successfully.
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
            if( pointer_ == NULLPTR )
            {
                break;
            }
            counter_ = new Counter(1);
            if( counter_ == NULLPTR )
            {
                D::free(pointer_);
                break;
            }            
            res = true;
        } while(false);
        return res;
    }

    /**
     * @brief Counter of copies of the shared pointer.
     *
     * @todo Do these operations under a mutex.
     */
    class Counter : public ObjectAllocator<A>
    {
        
    public:
        
        /**
         * @brief Constructor.
         *
         * @param counter Initial value of the counter.
         */
        explicit Counter(int32_t const counter) : 
            counter_ (counter){
        }

        /**
         * @brief Increases the counter.
         *
         * @return a value of the counter after increasing.
         */
        int32_t increase()
        {
            return counter_++;
        }
        
        /**
         * @brief Decreases the counter.
         *
         * @return a value of the counter after decreasing.
         */        
        int32_t decrease()
        {
            return counter_--;
        }
        
        /**
         * @brief Returns the counter.
         *
         * @return a value of the counter.
         */        
        int32_t get() const
        {
            return counter_;
        }
    
    private:
    
        int32_t counter_;
    };
    
    /**
     * @brief An owned pointer.
     */
    T* pointer_;
    
    /**
     * @brief Counter of copies of the object.
     */
    Counter* counter_;

};

#endif // EOOS_NO_STRICT_MISRA_RULES

} // namespace lib
} // namespace eoos
#endif // LIB_SHARED_PTR_HPP_
