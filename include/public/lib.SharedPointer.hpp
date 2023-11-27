/**
 * @file      lib.SharedPointer.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_SHAREDPOINTER_HPP_
#define LIB_SHAREDPOINTER_HPP_

#include "lib.Object.hpp"
#include "api.SmartPointer.hpp"
#include "lib.SmartPointerDeleter.hpp"
#include "lib.Guard.hpp"
#include "lib.Mutex.hpp"
#include "lib.NonCopyable.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class SharedPointer<T,D,A>
 * @brief Shared pointer.
 *
 * @tparam T Data type of an owning object.
 * @tparam D Deleter type for an owning object. 
 * @tparam A Heap memory allocator class.
 */
template <typename T, class D = SmartPointerDeleter<T>, class A = Allocator>
class SharedPointer : public Object<A>, public api::SmartPointer<T>
{
    typedef SharedPointer<T,D,A> Self;
    typedef Object<A> Parent;

public:

    /**
     * @brief Constructor an empty shared object.
     */
    SharedPointer() 
        : Object<A>()
        , api::SmartPointer<T>()
        , cb_(NULLPTR) {
        bool_t const isConstructed( construct() );
        setConstructed(isConstructed);    
    }

    /**
     * @brief Constructor.
     *
     * @note If the shared object is not able to be constructed, an object passed by the pointer will be deleted.
     *
     * @param pointer A pointer to get ownership.
     */
    explicit SharedPointer(T* const pointer) 
        : Object<A>()
        , api::SmartPointer<T>()
        , cb_(NULLPTR) {
        bool_t const isConstructed( construct(pointer) );
        setConstructed(isConstructed);    
    }

    /**
     * @brief Destructor.
     */
    virtual ~SharedPointer()
    {
        if( isConstructed() )
        {
            release();
        }
    }

    /**
     * @copydoc eoos::Object::Object(Object const&)
     */
    SharedPointer(SharedPointer const& obj) ///< SCA MISRA-C++:2008 Justified Rule 12-8-1
        : Object<A>(obj)
        , api::SmartPointer<T>()
        , cb_(obj.cb_){
        acquire();
    }

    /**
     * @copydoc eoos::Object::operator=(Object const&)
     */       
    SharedPointer& operator=(SharedPointer const& obj)
    {
        if( isConstructed() && (this != &obj) )
        {
            release();
            cb_ = obj.cb_;
            acquire();            
            Parent::operator=(obj);            
        }
        return *this;
    }    

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(Object&&)
     */       
    SharedPointer(SharedPointer&& obj) noexcept 
        : Object<A>( move(obj) )
        , api::SmartPointer<T>()        
        , cb_(obj.cb_) {
    }   

    /**
     * @copydoc eoos::Object::operator=(Object&&)
     */
    SharedPointer& operator=(SharedPointer&& obj) & noexcept
    {
        if( isConstructed() && (this != &obj) )
        {
            release();            
            cb_ = obj.cb_;
            Parent::operator=( move(obj) );            
        }        
        return *this;
    }        

    #endif // EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        return Parent::isConstructed();
    }

    /**
     * @brief Casts to boolean data type comparing if the stored pointer does not equal to null.
     *
     * @return Comparation the stored pointer does not equal to null.
     */    
    operator bool_t() const 
    {
        return get() != NULLPTR;
    }

    /**
     * @brief Returns the result of dereferencing the stored pointer.
     *
     * @return The dereferenced stored pointer.
     */
    T& operator*() const
    {
        return *get();
    }

    /**
     * @brief Returns the stored pointer.
     *
     * @return The stored pointer or NULLPTR if no pointer stored.
     */
    T* operator->() const
    {
        return get();
    }

    /**
     * @brief Returns an element of the stored array.
     *
     * @param index An element index.
     * @return An element.
     */    
    T& operator[](uint32_t const index) const
    {
        T* const pointer( get() );
        return pointer[index];
    }
    
    /**
     * @copydoc eoos::api::SmartPointer::get()
     */
    virtual T* get() const
    {
        T* pointer( NULLPTR );
        if( isConstructed() )
        {
            pointer = cb_->getPointer();
        }
        return pointer;
    }

    /**
     * @copydoc eoos::api::SmartPointer::reset()
     */   
    virtual void reset()
    {
        reset(NULLPTR);
    }

    /**
     * @copydoc eoos::api::SmartPointer::reset(T*)
     */
    virtual void reset(T* ptr)
    {
        if (get() != ptr)
        {
            Self temp(ptr);
            swap(temp);
        }
    }

    /**
     * @copydoc eoos::api::SmartPointer::getCount()
     */
    virtual int32_t getCount() const
    {
        int32_t counter( 0 );
        if( isConstructed() )
        {
            if( cb_->getPointer() != NULLPTR )
            {
                counter = cb_->getCounter();
            }
        }
        return counter;
    }
    
    /**
     * @copydoc eoos::api::SmartPointer::isNull()
     */       
    virtual bool_t isNull() const
    {
        return get() == NULLPTR;        
    }
    
    /**
     * @copydoc eoos::api::SmartPointer::isUnique()
     */   
    virtual bool_t isUnique() const
    {
        return getCount() == 1;
    }
    
    /**
     * @brief Swaps this managed object with an object managed by given smart object.
     *
     * @param obj A smart object to swap managed objects.
     */    
    void swap(SharedPointer& obj)
    {
        if( isConstructed() && obj.isConstructed() )
        {
            ControlBlock<T,D,A>* const cb( cb_ );
            cb_ = obj.cb_;
            obj.cb_ = cb;
        }
    }    
    
protected:

    using Parent::setConstructed;     

private:

    /**
     * @brief Constructs this object.
     *
     * @param pointer A pointer to get ownership.
     * @return True if this object has been constructed successfully.
     */     
    bool_t construct(T* const pointer = NULLPTR)
    {
        bool_t res( false );
        do
        {
            if( !isConstructed() )
            {   ///< UT Justified Branch: HW dependency
                D::free(pointer);
                break;
            }
            cb_ = new ControlBlock<T,D,A>(pointer);
            if(cb_ == NULLPTR)
            {
                D::free(pointer);
                break;
            }
            if( !cb_->isConstructed() )
            {   ///< UT Justified Branch: HW dependency
                D::free(pointer);                
                delete cb_;
                cb_ = NULLPTR;
                break;
            }
            res = true;
        } while(false);
        return res;
    }

    /**
     * @brief Releases the managed object by control block.
     */       
    void release()
    {
        if( cb_ != NULLPTR )
        {
            int32_t const counter( cb_->decrease() );
            if(counter == 0)
            {
                D::free(cb_->getPointer());
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
    }

    /**
     * @class ControlBlock<TT,DD,AA>
     * @brief Primary template implementation of shared pointer control block class.
     *
     * @tparam TT Data type of owning the object.
     * @tparam DD Deleter type for owning the object.  
     * @tparam AA Heap memory allocator class.
     *
     * @note This class is implemented as an auxiliry class for SharedPointer
     * and is tested for construction before usage. Therefore, some checks
     * are skipped in public interface to speedup performence. 
     */
    template <typename TT, class DD, class AA> 
    class ControlBlock : public NonCopyable<AA>
    {
        typedef NonCopyable<AA> Parent;    

    public:

        /**
         * @brief Constructor.
         *
         * @param pointer A pointer to get ownership.
         */
        explicit ControlBlock(T* const pointer) 
            : NonCopyable<AA>()
            , pointer_(pointer)
            , counter_(1)
            , mutex_() {
            bool_t const isConstructed( construct() );
            setConstructed(isConstructed);
        }

        /**
         * @copydoc eoos::api::Object::isConstructed()
         */
        virtual bool_t isConstructed() const
        {
            return Parent::isConstructed();
        }

        /**
         * @brief Destructor.
         */
        virtual ~ControlBlock() {}    

        /**
         * @brief Increases the counter on one.
         */
        void increase()
        {
            Guard<AA> const guard(mutex_);
            static_cast<void>(guard);            
            ++counter_;
        }

        /**
         * @brief Decreases the counter on one.
         *
         * @return A value of the counter after decreasing.
         */        
        int32_t decrease()
        {
            Guard<AA> const guard(mutex_);
            static_cast<void>(guard);            
            return --counter_;
        }

        /**
         * @brief Returns the counter.
         *
         * @return A value of the counter.
         */        
        int32_t getCounter() const
        {    
            return counter_;
        }

        /**
         * @brief Returns the managed raw pointer.
         *
         * @return The managed raw pointer.
         */        
        TT* getPointer() const
        {
            return pointer_;
        }    

    private:
    
        /**
         * @brief Constructs this object.
         */     
        bool_t construct()
        {
            bool_t res( false );
            do
            {
                if( !isConstructed() )
                {   ///< UT Justified Branch: HW dependency
                    break;
                }
                if( !mutex_.isConstructed() )
                {   ///< UT Justified Branch: HW dependency
                    break;
                }
                res = true;
            } while(false);
            return res;
        }
        
        /**
         * @brief An owned pointer.
         */
        TT* pointer_;
        
        /**
         * @brief Counter of copies of the shared objects.
         */
        int32_t counter_;
    
        /**
         * @brief Mutex to protect the counter.
         */    
        Mutex<AA> mutex_;
    };

    /**
     * @brief Control block of the managed object.
     */
    ControlBlock<T,D,A>* cb_;

};

/**
 * @brief Comparison operator to equal.
 *
 * @param obj1 Reference to object.
 * @param obj2 Reference to object.
 * @return True if objects are equal.
 */
template <typename T, class D = SmartPointerDeleter<T>, class A = Allocator>
inline bool_t operator==(SharedPointer<T,D,A> const& obj1, SharedPointer<T,D,A> const& obj2)
{
    return obj1.get() == obj2.get();
}

/**
 * @brief Comparison operator to unequal.
 *
 * @param obj1 Reference to object.
 * @param obj2 Reference to object.
 * @return True if objects are not equal.
 */
template <typename T, class D = SmartPointerDeleter<T>, class A = Allocator>
inline bool_t operator!=(SharedPointer<T,D,A> const& obj1, SharedPointer<T,D,A> const& obj2)
{
    return obj1.get() != obj2.get();
}

} // namespace lib
} // namespace eoos
#endif // LIB_SHAREDPOINTER_HPP_
