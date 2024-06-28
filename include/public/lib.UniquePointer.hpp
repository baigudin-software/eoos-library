/**
 * @file      lib.UniquePointer.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2022-2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_UNIQUEPOINTER_HPP_
#define LIB_UNIQUEPOINTER_HPP_

#include "lib.Object.hpp"
#include "api.SmartPointer.hpp"
#include "lib.SmartPointerDeleter.hpp"

namespace eoos
{
namespace lib
{
   
/**
 * @class UniquePointer<T,D,A>
 * @brief Unique pointer.
 *
 * @tparam T Data type of an owning object.
 * @tparam D Deleter type for an owning object. 
 * @tparam A Heap memory allocator class.
 */
template <typename T, class D = SmartPointerDeleter<T>, class A = Allocator>
class UniquePointer : public Object<A>, public api::SmartPointer<T>
{
    typedef UniquePointer<T,D,A> Self;
    typedef Object<A> Parent;

public:

    /**
     * @brief Constructor of an empty unique object.
     */
    UniquePointer();

    /**
     * @brief Constructor.
     *
     * @note If the unique object is not able to be constructed, an object passed by the pointer will be deleted.
     *
     * @param pointer A pointer to get ownership.
     */
    explicit UniquePointer(T* const pointer);

    /**
     * @brief Destructor.
     */
    virtual ~UniquePointer();

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(Object&&)
     */       
    UniquePointer(UniquePointer&& obj) noexcept;

    /**
     * @copydoc eoos::Object::operator=(Object&&)
     */
    UniquePointer& operator=(UniquePointer&& obj) & noexcept;

    #endif // EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const;

    /**
     * @brief Casts to boolean data type comparing if the stored pointer does not equal to null.
     *
     * @return Comparation the stored pointer does not equal to null.
     */    
    operator bool_t() const;

    /**
     * @brief Returns the result of dereferencing the stored pointer.
     *
     * @return The dereferenced stored pointer.
     */
    T& operator*() const;

    /**
     * @brief Returns the stored pointer.
     *
     * @return The stored pointer or NULLPTR if no pointer stored.
     */
    T* operator->() const;

    /**
     * @brief Returns an element of the stored array.
     *
     * @param index An element index.
     * @return An element.
     */    
    T& operator[](uint32_t const index) const;

    /**
     * @copydoc eoos::api::SmartPointer::get()
     */
    virtual T* get() const;

    /**
     * @copydoc eoos::api::SmartPointer::reset()
     */   
    virtual void reset();

    /**
     * @copydoc eoos::api::SmartPointer::reset(T*)
     */
    virtual void reset(T* ptr);

    /**
     * @copydoc eoos::api::SmartPointer::getCount()
     */
    virtual int32_t getCount() const;
    
    /**
     * @copydoc eoos::api::SmartPointer::isNull()
     */       
    virtual bool_t isNull() const;
    
    /**
     * @copydoc eoos::api::SmartPointer::isUnique()
     */   
    virtual bool_t isUnique() const;
    
    /**
     * @brief Swaps this managed object with an object managed by given smart object.
     *
     * @param obj A smart object to swap managed objects.
     */
    void swap(UniquePointer& obj);
    
    /**
     * @brief Releases ownership of the managed object.
     *
     * @return Released raw pointer.
     */    
    T* release();
    
protected:

    using Parent::setConstructed;     

private:

    /**
     * @brief Constructs this object.
     *
     * @param pointer A pointer to get ownership.
     * @return True if this object has been constructed successfully.
     */     
    bool_t construct(T* const pointer = NULLPTR);
    
    /**
     * @brief Releases the managed object by control block.
     */       
    void releasePointer();
    
    #if EOOS_CPP_STANDARD < 2011
    
    /**
     * @copydoc eoos::Object::Object(Object const&)
     */
    UniquePointer(UniquePointer const&);

    /**
     * @copydoc eoos::Object::operator=(Object const&)
     */       
    UniquePointer& operator=(UniquePointer const&);    

    #endif // EOOS_CPP_STANDARD < 2011

    /**
     * @brief Pointer to the managed object.
     */
    T* pointer_;

};

/**
 * @brief Comparison operator to equal.
 *
 * @param obj1 Reference to object.
 * @param obj2 Reference to object.
 * @return True if objects are equal.
 */
template <typename T, class D, class A>
inline bool_t operator==(UniquePointer<T,D,A> const& obj1, UniquePointer<T,D,A> const& obj2)
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
template <typename T, class D, class A>
inline bool_t operator!=(UniquePointer<T,D,A> const& obj1, UniquePointer<T,D,A> const& obj2)
{
    return obj1.get() != obj2.get();
}

template <typename T, class D, class A>
UniquePointer<T,D,A>::UniquePointer() 
    : Object<A>()
    , api::SmartPointer<T>()
    , pointer_ (NULLPTR) {
    bool_t const isConstructed( construct() );
    setConstructed(isConstructed);    
}

template <typename T, class D, class A>
UniquePointer<T,D,A>::UniquePointer(T* const pointer) 
    : Object<A>()
    , api::SmartPointer<T>()
    , pointer_ (NULLPTR) {
    bool_t const isConstructed( construct(pointer) );
    setConstructed(isConstructed);    
}

template <typename T, class D, class A>
UniquePointer<T,D,A>::~UniquePointer()
{
    if( isConstructed() )
    {
        releasePointer();
    }
}

#if EOOS_CPP_STANDARD >= 2011

template <typename T, class D, class A>
UniquePointer<T,D,A>::UniquePointer(UniquePointer&& obj) noexcept 
    : Object<A>( move(obj) )
    , api::SmartPointer<T>()
    , pointer_ (obj.pointer_) {
}   

template <typename T, class D, class A>
UniquePointer<T,D,A>& UniquePointer<T,D,A>::operator=(UniquePointer&& obj) & noexcept
{
    if( this != &obj && isConstructed() )
    {
        releasePointer();
        pointer_ = obj.pointer_;
        Parent::operator=( move(obj) );            
    }        
    return *this;
}        

#endif // EOOS_CPP_STANDARD >= 2011

template <typename T, class D, class A>
bool_t UniquePointer<T,D,A>::isConstructed() const ///< SCA MISRA-C++:2008 Justified Rule 10-3-1 and Defected Rule 9-3-3
{
    return Parent::isConstructed();
}

template <typename T, class D, class A>
UniquePointer<T,D,A>::operator bool_t() const 
{
    return get() != NULLPTR;
}

template <typename T, class D, class A>
T& UniquePointer<T,D,A>::operator*() const
{
    return *get();
}

template <typename T, class D, class A>
T* UniquePointer<T,D,A>::operator->() const
{
    return get();
}

template <typename T, class D, class A>
T& UniquePointer<T,D,A>::operator[](uint32_t const index) const
{
    T* const pointer( get() );
    return pointer[index];
}

template <typename T, class D, class A>
T* UniquePointer<T,D,A>::get() const
{
    T* pointer( NULLPTR );
    if( isConstructed() )
    {
        pointer = pointer_;
    }
    return pointer;
}

template <typename T, class D, class A>
void UniquePointer<T,D,A>::reset()
{
    reset(NULLPTR);
}

template <typename T, class D, class A>
void UniquePointer<T,D,A>::reset(T* ptr)
{
    if (get() != ptr)
    {
        Self temp(ptr);
        swap(temp);
    }
}

template <typename T, class D, class A>
int32_t UniquePointer<T,D,A>::getCount() const
{
    int32_t counter( 0 );
    if( isConstructed() && (pointer_ != NULLPTR) )
    {
        counter = 1;
    }
    return counter;
}

template <typename T, class D, class A>
bool_t UniquePointer<T,D,A>::isNull() const
{
    return get() == NULLPTR;        
}

template <typename T, class D, class A>
bool_t UniquePointer<T,D,A>::isUnique() const
{
    return getCount() == 1;
}

template <typename T, class D, class A>
void UniquePointer<T,D,A>::swap(UniquePointer& obj)
{
    if( isConstructed() )
    {
        if( obj.isConstructed() )
        {
            T* const pointer( pointer_ );
            pointer_ = obj.pointer_;
            obj.pointer_ = pointer;
        }
    }
}

template <typename T, class D, class A>
T* UniquePointer<T,D,A>::release()
{
    T* pointer( pointer_ );
    if( isConstructed() )
    {
        pointer_ = NULLPTR;
    }
    return pointer;
}

template <typename T, class D, class A>
bool_t UniquePointer<T,D,A>::construct(T* const pointer)
{
    bool_t res( false );
    if( isConstructed() )
    {
        pointer_ = pointer;
        res = true;
    }
    else
    {   ///< UT Justified Branch: HW dependency
        D::free(pointer);
    }
    return res;
}

template <typename T, class D, class A>
void UniquePointer<T,D,A>::releasePointer()
{
    D::free(pointer_);
    pointer_ = NULLPTR;
}       

} // namespace lib
} // namespace eoos
#endif // LIB_UNIQUEPOINTER_HPP_
