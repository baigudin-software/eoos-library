/**
 * @brief Root class of the library class hierarchy.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2020, Sergey Baigudin, Baigudin Software
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
 * @brief Primary template implementation.
 *
 * @param A - heap memory allocator class.
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
     * @brief Copy constructor.
     *
     * @param obj - reference to a source object.
     */
    Object(const Object& obj) : Parent(obj) {} 
    
    /**
     * @brief Copy assignment operator.
     *
     * @param obj - reference to a source object.
     * @return reference to this object.
     */       
    Object& operator=(const Object& obj)
    {
        Parent::operator=(obj);
        return *this;
    }    

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @brief Move constructor.
     *
     * @param obj - right reference to a source object.     
     */       
    Object(Object&& obj) noexcept : Parent( move(obj) ) {}
    
    /**
     * @brief Move assignment operator.
     *
     * @param obj - right reference to a source object.
     * @return reference to this object.
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
