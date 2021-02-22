/**
 * @brief Stack.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_STACK_HPP_
#define LIB_STACK_HPP_

#include "lib.Object.hpp"
#include "api.Stack.hpp"
#include "lib.Buffer.hpp"

namespace eoos
{
namespace lib
{

/**
 * @brief Primary template implementation.
 *
 * @tparam T data type of default stack element.
 * @tparam A heap memory allocator class.
 */
template <typename T, class A = Allocator>
class Stack : public Object<A>, public api::Stack<T>
{
    typedef ::eoos::lib::Object<A> Parent;
    typedef api::Stack<T> StackIntf;

public:

    /**
     * @brief Constructor.
     *
     * @param type  type of this stack.
     * @param count count of buffer elements.
     */
    Stack(typename api::Stack<T>::Operation type, int32_t count) : Parent(),
        stack_ (count),
        type_  (type){
        const bool_t isConstructed = construct();
        this->setConstruct( isConstructed );
    }

    /**
     * @brief Constructor.
     *
     * @param type  type of this stack.
     * @param count   count of buffer elements.
     * @param illegal illegal value.
     */
    Stack(typename api::Stack<T>::Operation type, int32_t count, const T illegal) : Parent(),
        stack_ (count, illegal),
        type_  (type){
        const bool_t isConstructed = construct();
        this->setConstruct( isConstructed );
    }

    /**
     * @brief Destructor.
     */
    virtual ~Stack()
    {
    }

    /**
     * @brief Tests if this object has been constructed.
     *
     * @return true if object has been constructed successfully.
     */
    virtual bool_t isConstructed() const
    {
        return this->isConstructed_;
    }

    /**
     * @brief Returns an initial top of stack.
     *
     * @return pointer to TOS.
     */
    virtual const T* getTos()
    {
        if( not this->isConstructed_ )
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
     * @brief Returns an type of stack operation.
     *
     * @return the stack operation.
     */
    virtual typename api::Stack<T>::Operation getType() const
    {
        return type_;
    }

    /**
     * @brief Returns a number of elements.
     *
     * @return number of elements.
     */
    virtual int32_t getLength() const
    {
        return stack_.getLength();
    }

    /**
     * @brief Tests if this collection has elements.
     *
     * @return true if this collection does not contain any elements.
     */
    virtual bool_t isEmpty() const
    {
        return false;
    }

    /**
     * @brief Returns illegal element which will be returned as error value.
     *
     * If illegal value is not set method returns uninitialized variable.
     *
     * @return reference to illegal element.
     */
    virtual const T& getIllegal() const
    {
        return stack_.getIllegal();
    }

    /**
     * @brief Sets illegal element which will be returned as error value.
     *
     * @param value illegal value.
     */
    virtual void setIllegal(const T& value)
    {
        return stack_.setIllegal(value);
    }

    /**
     * @brief Tests if given value is an illegal.
     *
     * @param value testing value.
     * @param true if value is an illegal.
     */
    virtual bool_t isIllegal(const T& value) const
    {
        return stack_.isIllegal(value);
    }

private:

    /**
     * @brief Constructor.
     *
     * @return true if object has been constructed successfully.
     */
    bool_t construct()
    {
        if( not this->isConstructed_ )
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
     * @brief Copy constructor.
     *
     * @param obj reference to source object.
     */
    Stack(const Stack& obj);

    /**
     * @brief Assignment operator.
     *
     * @param obj reference to source object.
     * @return reference to this object.
     */
    Stack& operator=(const Stack& obj);

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
