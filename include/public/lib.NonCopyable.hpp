/**
 * @file      lib.NonCopyable.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_NONCOPYABLE_HPP_
#define LIB_NONCOPYABLE_HPP_

#include "lib.Object.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class NonCopyable
 * @brief Root class of the operating system for objects which cannot be copied.
 *
 * @tparam A Heap memory allocator class. 
 */
template <class A = Allocator>
class NonCopyable : public Object<A>
{
    typedef lib::Object<A> Parent;

public:

    /**
     * @brief Constructor.
     */
    NonCopyable() : Parent() {}

    /**
     * @brief Destructor.
     */
    virtual ~NonCopyable() {}
    
private:
    
    /**
     * @copydoc eoos::Object::Object(Object const&)
     */
    NonCopyable(NonCopyable const& obj);
    
    /**
     * @copydoc eoos::Object::operator=(Object const&)
     */       
    NonCopyable& operator=(NonCopyable const& obj);   

    #if EOOS_CPP_STANDARD >= 2011
    
    /**
     * @copydoc eoos::Object::Object(Object&&)
     */       
    NonCopyable(NonCopyable&& obj) noexcept = delete;
    
    /**
     * @copydoc eoos::Object::operator=(Object&&)
     */
    NonCopyable& operator=(NonCopyable&& obj) noexcept = delete;

    #endif // EOOS_CPP_STANDARD >= 2011

};

} // namespace sys
} // namespace eoos
#endif // SYS_NONCOPYABLE_HPP_
