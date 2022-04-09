/**
 * @file      lib.Stack.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_STACK_HPP_
#define LIB_STACK_HPP_

#include "lib.NonCopyable.hpp"
#include "api.Stack.hpp"
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
class Stack : public NonCopyable<A>, public api::Stack<T>
{
    typedef NonCopyable<A> Parent;
    typedef api::Stack<T> StackIntf;

public:

    /**
     * @brief Constructor.
     *
     * @param type  Type of this stack.
     * @param count Count of buffer elements.
     */
    Stack(typename api::Stack<T>::Operation type, int32_t count) : Parent(),
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
    Stack(typename api::Stack<T>::Operation type, int32_t count, const T illegal) : Parent(),
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
     * @copydoc eoos::api::Stack::getTos()
     */
    virtual const T* getTos()
    {
        if( not isConstructed() )
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
     * @copydoc eoos::api::Stack::getType()
     */
    virtual typename api::Stack<T>::Operation getType() const
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
    
protected:

    using Parent::setConstructed;    

private:

    /**
     * @brief Constructor.
     *
     * @return True if object has been constructed successfully.
     */
    bool_t construct()
    {
        if( not isConstructed() )
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
     * @brief Stack memory buffer.
     */
    Buffer<T,0,A> stack_;

    /**
     * @brief Stack type.
     */
    const typename api::Stack<T>::Operation type_;

};
        
} // namespace lib
} // namespace eoos
#endif // LIB_STACK_HPP_
