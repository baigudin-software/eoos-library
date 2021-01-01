/**
 * @brief Shared pointer.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_SHARED_PTR_HPP_
#define LIB_SHARED_PTR_HPP_

#include "lib.Object.hpp"
#include "lib.aux.ControlBlock.hpp"

namespace eoos
{
namespace lib
{

#ifdef EOOS_NO_STRICT_MISRA_RULES

/**
 * @brief Deleter of shared pointers allocate with new operator.
 *
 * @tparam T Data type of an owning object. 
 */
template <typename T>
class SharedPtrDeleter
{
    
public:

    /**
     * @brief Frees an allocated object.
     *
     * @param ptr Address of allocated the owning object.
     */
	static void free(T* const ptr)
	{
		delete ptr;
	}
};

/**
 * @brief Deleter of shared pointers allocate with new [] operator.
 *
 * @tparam T Data type of an owning object. 
 */
template <typename T>
class SharedPtrDeleterArray
{
    
public:

    /**
     * @brief Frees an allocated array of objects.
     *
     * @param ptr Address of allocated the owning objects.
     */
	static void free(T* const ptr)
	{
		delete [] ptr;
	}
};
    
/**
 * @brief Primary template implementation.
 *
 * @tparam T Data type of an owning object.
 * @tparam D Deleter type for an owning object. 
 * @tparam A Heap memory allocator class.
 */
template <typename T, class D = SharedPtrDeleter<T>, class A = Allocator>
class SharedPtr : public Object<A>
{
    typedef SharedPtr<T,A>  Self;
    typedef Object<A> Parent;

public:

    /**
     * @brief Constructor an empty shared object.
     */
    explicit SharedPtr() : Parent(),
        cb_ (NULLPTR){
        bool_t const isConstructed = construct();
        setConstructed(isConstructed);    
    }

    /**
     * @brief Constructor.
     *
     * @param pointer A pointer to get ownership.
     * @note If the shared object is not able to be constructed, an object passed by the pointer will be deleted.
     */
    explicit SharedPtr(T* const pointer) : Parent(),
        cb_ (NULLPTR){
        bool_t const isConstructed = construct(pointer);
        setConstructed(isConstructed);    
    }

    /**
     * @brief Destructor.
     */
    virtual ~SharedPtr()
    {
        if( isConstructed() )
        {
            release();
        }
    }
    
    /**
     * @brief Copy constructor.
     *
     * @param obj Reference to a source object.
     */
    SharedPtr(const SharedPtr& obj) : Parent(obj),
        cb_ (obj.cb_){
        acquire();
    }
    
    /**
     * @brief Copy assignment operator.
     *
     * @param obj   Reference to a source object.
     * @return reference to this object.
     */       
    SharedPtr& operator=(const SharedPtr& obj)
    {
        if( this != &obj && isConstructed() )
        {
            release();
            Parent::operator=(obj);            
            cb_ = obj.cb_;
            acquire();
        }
        return *this;
    }    

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @brief Move constructor.
     *
     * @param obj Right reference to a source object.     
     */       
    SharedPtr(SharedPtr&& obj) noexcept : Parent( move(obj) ),
        cb_ (obj.cb_){
    }   
    
    /**
     * @brief Move assignment operator.
     *
     * @param obj Right reference to a source object.
     * @return reference to this object.
     */
    SharedPtr& operator=(SharedPtr&& obj) noexcept
    {
        if( this != &obj && isConstructed() )
        {
            Parent::operator=( move(obj) );            
            cb_ = obj.cb_;
        }        
        return *this;
    }        
    
    #endif // EOOS_CPP_STANDARD >= 2011

    /**
     * @brief Casts to boolean data type comparing if the stored pointer does not equal to null.
     *
     * @return comparation the stored pointer does not equal to null.
     */    
    operator bool_t() const 
    {
        return get() != NULLPTR;
    }
    
    /**
     * @brief Returns the result of dereferencing the stored pointer.
     *
     * @return the dereferenced stored pointer.
     */
    T& operator*() const
    {
        return *get();
    }
    
    /**
     * @brief Returns the stored pointer.
     *
     * @return the stored pointer or NULLPTR if no pointer stored.
     */
    T* operator->() const
    {
        return get();
    }
    
    /**
     * @brief Returns an element of the stored array.
     *
     * @param index An element index.
     * @return an element.
     */    
    T& operator[](uint32_t const index) const
    {
        T* pointer = get();
        return pointer[index];
    }
    
    /**
     * @brief Returns the stored pointer.
     *
     * @return the stored pointer or NULLPTR if no pointer stored.
     */   
    T* get() const
    {
        T* pointer = NULLPTR;
        if( isConstructed() )
        {
            pointer = cb_->getPointer();
        }
        return pointer;
    }
    
    /**
     * @brief Returns counter of shared objects for the managed object.
     *
     * @return counter of shared objects.
     */   
    uint32_t getCount() const
    {
        uint32_t counter = 0;
        if( isConstructed() )
        {
            counter = cb_->getCounter();
        }
        return counter;
    }    
        
private:

    /**        
     * @brief Constructs this object.
     *
     * @param pointer A pointer to get ownership.
     * @return true if this object has been constructed successfully.
     */     
    bool_t construct(T* const pointer = NULLPTR)
    {
        bool_t res = false;
        do
        {
            if( not isConstructed() )
            {
                D::free(pointer);
                break;
            }
            cb_ = new aux::ControlBlock<T,D,A>(pointer);
            if(cb_ == NULLPTR)
            {
                D::free(pointer);
                break;
            }
            if( not cb_->isConstructed() )
            {
                delete cb_;
                cb_ = NULLPTR;
                break;
            }
            res = true;
        } while(false);
        return res;
    }
    
    /**
     * @brief Release the managed object by control block.
     */       
    void release()
    {
        if( cb_ != NULLPTR )
        {
            uint32_t const counter = cb_->decrease();
            if(counter == 0)
            {
                delete cb_;
                cb_ = NULLPTR;            
            }
        }
    }        
    
    /**
     * @brief Acquires a managed object by control block.
     */
    void acquire()
    {
        if( cb_ != NULLPTR )
        {
            cb_->increase();
        }
        else
        {
            setConstructed(false);
        }
    }
       
    /**
     * @brief Control block of the managed object.
     */
    aux::ControlBlock<T,D,A>* cb_;

};

#endif // EOOS_NO_STRICT_MISRA_RULES

} // namespace lib
} // namespace eoos
#endif // LIB_SHARED_PTR_HPP_
