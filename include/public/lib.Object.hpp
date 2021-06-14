/**
 * @file      lib.Object.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2021, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_OBJECT_HPP_
#define LIB_OBJECT_HPP_

#include "Object.hpp"
#include "lib.Types.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class Object<A>
 * @brief Root class of the library class hierarchy.
 *
 * @tparam A Heap memory allocator class.
 */
template <class A = Allocator>
class Object : public ::eoos::Object<A>
{
    typedef ::eoos::Object<A> Parent;

public:

    /**
     * @brief Constructor.
     */
    Object() : Parent() {}

    /**
     * @brief Destructor.
     */
    virtual ~Object() {}
    
    /**
     * @copydoc eoos::Object::Object(const Object&)
     */
    Object(const Object& obj) : Parent(obj) {} 
    
    /**
     * @copydoc eoos::Object::operator=(const Object&)
     */       
    Object& operator=(const Object& obj)
    {
        Parent::operator=(obj);
        return *this;
    }    

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(const Object&&)
     */       
    Object(Object&& obj) noexcept : Parent( move(obj) ) {}
    
    /**
     * @copydoc eoos::Object::operator=(const Object&&)
     */
    Object& operator=(Object&& obj) noexcept
    {
        Parent::operator=( move(obj) );
        return *this;
    }        
    
    #endif // EOOS_CPP_STANDARD >= 2011    

};

} // namespace lib
} // namespace eoos
#endif // LIB_OBJECT_HPP_
