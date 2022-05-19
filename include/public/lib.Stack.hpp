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
    Stack(typename api::Stack<T>::Operation type, int32_t count) 
        : NonCopyable<A>()
        , api::Stack<T>()
        , stack_(count)
        , type_(type){
        bool_t const isConstructed( construct() );
        setConstructed( isConstructed );
    }

    /**
     * @brief Constructor.
     *
     * @param type    Type of this stack.
     * @param count   Count of buffer elements.
     * @param illegal Illegal value.
     */
    Stack(typename api::Stack<T>::Operation type, int32_t count, T const illegal) 
        : NonCopyable<A>()
        , api::Stack<T>()
        , stack_(count, illegal)
        , type_(type) {
        bool_t const isConstructed( construct() );
        setConstructed( isConstructed );
    }

    /**
     * @brief Destructor.
     */
    virtual ~Stack(){}

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        return Parent::isConstructed();
    }

    /**
     * @copydoc eoos::api::Stack::getTos()
     */
    virtual T const* getTos()
    {
        if( !isConstructed() )
        {
            return NULLPTR;
        }
        T* const stack( &stack_[0] );
        T* tos( NULLPTR );
        switch(type_)
        {
            case StackIntf::FD:
            {
                tos = &stack[stack_.getLength()];
                break;
            }
            case StackIntf::ED:
            {
                tos = &stack[stack_.getLength() - 1];
                break;
            }
            case StackIntf::FA:
            {
                tos = &stack[0] - 1;
                break;
            }
            case StackIntf::EA:
            {
                tos = &stack[0];
                break;
            }
            default:
            {
                tos = NULLPTR;
                break;
            }
        }
        return tos;
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
    virtual bool_t isEmpty() const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        return false;
    }

    /**
     * @copydoc eoos::api::IllegalValue::getIllegal()
     */
    virtual T const& getIllegal() const
    {
        return stack_.getIllegal();
    }

    /**
     * @copydoc eoos::api::IllegalValue::setIllegal(T const&)
     */
    virtual void setIllegal(T const& value)
    {
        return stack_.setIllegal(value);
    }

    /**
     * @copydoc eoos::api::IllegalValue::isIllegal(T const&)
     */
    virtual bool_t isIllegal(T const& value) const
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
        if( !isConstructed() )
        {
            return false;
        }
        if( !stack_.isConstructed() )
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
