/**
 * @file      lib.Stack.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2021, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_STACK_HPP_
#define LIB_STACK_HPP_

#include "lib.Object.hpp"
#include "api.SysStack.hpp"
#include "lib.Buffer.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class Stack<T,A>
 * @brief Stack implementation for system usage.
 *
 * @tparam T data type of default stack element.
 * @tparam A Heap memory allocator class.
 */
template <typename T, class A = Allocator>
class Stack : public Object<A>, public api::SysStack<T>
{
    typedef ::eoos::lib::Object<A> Parent;
    typedef api::SysStack<T> StackIntf;

public:

    /**
     * @brief Constructor.
     *
     * @param type  Type of this stack.
     * @param count Count of buffer elements.
     */
    Stack(typename api::SysStack<T>::Operation type, int32_t count) : Parent(),
        stack_ (count),
        type_  (type){
        bool_t const isConstructed = construct();
        setConstructed( isConstructed );
    }

    /**
     * @brief Constructor.
     *
     * @param type    Type of this stack.
     * @param count   Count of buffer elements.
     * @param illegal Illegal value.
     */
    Stack(typename api::SysStack<T>::Operation type, int32_t count, const T illegal) : Parent(),
        stack_ (count, illegal),
        type_  (type){
        bool_t const isConstructed = construct();
        setConstructed( isConstructed );
    }

    /**
     * @brief Destructor.
     */
    virtual ~Stack(){}

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }

    /**
     * @copydoc eoos::api::SysStack::getTos()
     */
    virtual const T* getTos()
    {
        if( not Self::isConstructed() )
        {
            return NULLPTR;
        }
        T* const stack = &stack_[0];
        switch(type_)
        {
            case StackIntf::FD:
                return &stack[stack_.getLength()];

            case StackIntf::ED:
                return &stack[stack_.getLength() - 1];

            case StackIntf::FA:
                return &stack[0] - 1;

            case StackIntf::EA:
                return &stack[0];

            default:
                return NULLPTR;
        }
    }

    /**
     * @copydoc eoos::api::SysStack::getType()
     */
    virtual typename api::SysStack<T>::Operation getType() const
    {
        return type_;
    }

    /**
     * @copydoc eoos::api::Collection::getLength()
     */
    virtual int32_t getLength() const
    {
        return stack_.getLength();
    }

    /**
     * @copydoc eoos::api::Collection::isEmpty()
     */
    virtual bool_t isEmpty() const
    {
        return false;
    }

    /**
     * @copydoc eoos::api::IllegalValue::getIllegal()
     */
    virtual const T& getIllegal() const
    {
        return stack_.getIllegal();
    }

    /**
     * @copydoc eoos::api::IllegalValue::setIllegal(const T&)
     */
    virtual void setIllegal(const T& value)
    {
        return stack_.setIllegal(value);
    }

    /**
     * @copydoc eoos::api::IllegalValue::isIllegal(const T&)
     */
    virtual bool_t isIllegal(const T& value) const
    {
        return stack_.isIllegal(value);
    }

private:

    /**
     * @brief Constructor.
     *
     * @return True if object has been constructed successfully.
     */
    bool_t construct()
    {
        if( not Self::isConstructed() )
        {
            return false;
        }
        if( not stack_.isConstructed() )
        {
            return false;
        }
        #ifdef EOOS_DEBUG
        stack_.fill(0);
        #endif
        return true;
    }

    /**
     * @copydoc eoos::Object::Object(const Object&)
     */
    Stack(const Stack& obj);

    /**
     * @copydoc eoos::Object::operator=(const Object&)
     */
    Stack& operator=(const Stack& obj);
    
    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(const Object&&)
     */       
    Stack(Stack&& obj) noexcept = delete; 
    
    /**
     * @copydoc eoos::Object::operator=(const Object&&)
     */
    Stack& operator=(Stack&& obj) noexcept = delete;
    
    #endif // EOOS_CPP_STANDARD >= 2011

    /**
     * @brief Stack memory buffer.
     */
    Buffer<T,0,A> stack_;

    /**
     * @brief Stack type.
     */
    const typename api::SysStack<T>::Operation type_;

};
        
} // namespace lib
} // namespace eoos
#endif // LIB_STACK_HPP_
