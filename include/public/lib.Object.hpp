/**
 * @file      lib.Object.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2022, Sergey Baigudin, Baigudin Software
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
    Object() 
        : ::eoos::Object<A>() {
    }

    /**
     * @brief Destructor.
     */
    virtual ~Object() {}
    
    /**
     * @copydoc eoos::Object::Object(Object const&)
     */
    Object(Object const& obj) 
        : ::eoos::Object<A>(obj) {
    } 
    
    /**
     * @copydoc eoos::Object::operator=(Object const&)
     */       
    Object& operator=(Object const& obj)
    {
        Parent::operator=(obj);
        return *this;
    }    

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(Object&&)
     */       
    Object(Object&& obj) noexcept 
        : ::eoos::Object<A>( move(obj) ) {
    }
    
    /**
     * @copydoc eoos::Object::operator=(Object&&)
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
