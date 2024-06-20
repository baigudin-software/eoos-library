/**
 * @file      lib.AbstractBaseString.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017-2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ABSTRACTBASESTRING_HPP_
#define LIB_ABSTRACTBASESTRING_HPP_

#include "lib.Object.hpp"
#include "api.String.hpp"
#include "lib.CharTrait.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class AbstractBaseString<T,R,A>
 * @brief Abstract base string class.
 *
 * @tparam T A data type of string characters.
 * @tparam R A character traits.
 * @tparam A A heap memory allocator class.
 */
template <typename T, class R, class A = Allocator>
class AbstractBaseString : public Object<A>, public api::String<T>
{
    typedef Object<A> Parent;

public:

    using api::String<T>::getLength;

    /**
     * @brief Destructor.
     */
    virtual ~AbstractBaseString();

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const;

    /**
     * @copydoc eoos::api::Collection::isEmpty()
     */
    virtual bool_t isEmpty() const;

    /**
     * @copydoc eoos::api::String::copy(const api::String<T>&)
     */
    virtual bool_t copy(api::String<T> const& string);

    /**
     * @copydoc eoos::api::String::concatenate(const api::String<T>&)
     */
    virtual bool_t concatenate(api::String<T> const& string);

    /**
     * @copydoc eoos::api::String::isEqualTo(const api::String<T>&)
     */
    virtual bool_t isEqualTo(api::String<T> const& string) const;
    
    /**
     * @brief Converts an integer number to this string.
     *
     * The function converts an integer value into a character string using the base parameter,
     * which has to be 2, 8, 10, or 16 based numerals for converting to an appropriate numeral system.
     *
     * Mark that only if the base is decimal, a passed number is available to be negative values,
     * and the resulting string of these values is preceded with a minus sign. In addition,
     * a hexadecimal number includes lower case characters, and any resulting strings do not contain
     * any suffixes or prefixes for identifying a numeral system.
     *
     * @note You need to use `AbstractBaseString.template convert<I>(value, base);` syntax,
     * if you have to specify the template argument type explicitly.
     *
     * @param value A value that would be converted to this string.
     * @param base  A numerical base used to represent a value as this string.
     * @return True if the conversion has been completed successfully.
     */
    template <typename I>
    bool_t convert(I const value, Number::Base const base = Number::BASE_10);

    /**
     * @brief Assignment by sum operator.
     *
     * @param source A source object interface.
     * @return Reference to this object.
     */
    AbstractBaseString& operator+=(api::String<T> const& source);

    /**
     * @brief Assignment by sum operator.
     *
     * @param source A source character string.
     * @return Reference to this object.
     */
    AbstractBaseString& operator+=(T const* const source);

protected:

    /**
     * @brief Constructor.
     */
    AbstractBaseString();

    /**
     * @copydoc eoos::Object::Object(Object const&)
     */
    AbstractBaseString(AbstractBaseString const& obj);

    /**
     * @copydoc eoos::Object::operator=(Object const&)
     */       
    AbstractBaseString& operator=(AbstractBaseString const& obj);

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(Object&&)
     */       
    AbstractBaseString(AbstractBaseString&& obj) noexcept;

    /**
     * @copydoc eoos::Object::operator=(Object&&)
     */
    AbstractBaseString& operator=(AbstractBaseString&& obj) & noexcept;

    #endif // EOOS_CPP_STANDARD >= 2011

    /**
     * @brief Returns a calculated string length.
     *
     * @param str A character string would be measured.
     * @return A length of the passed string.
     */
    static size_t getLengthRaw(T const* str);

    /**
     * @brief Copies a string to a string.
     *
     * @param dst A destination array where the content would be copied.
     * @param src A character string to be copied.
     * @param cnt Destination character string size.     
     */
    static void copyRaw3(T* dst, T const* src, size_t cnt);

    /**
     * @brief Concatenates two strings.
     *
     * @param dst A destination character string where the content would be appended.
     * @param src An appended character string.
     * @param cnt Destination character string length that cannot be less than dst string length.     
     */
    static void concatenateRaw3(T* dst, T const* src, size_t cnt);
    
    /**
     * @brief Compares two strings.
     *
     * @param string A string object interface to be compared with.
     * @return true if strings equal to each other.
     */
    static bool_t isEqualRaw2(T const* str1, T const* str2);

private:

    /**
     * @brief Copies a passed string into this string.
     *
     * @param str A character string to be copied.
     * @return True if a passed string has been copied successfully.
     */
    virtual bool_t copyRaw(T const* str) = 0;

    /**
     * @brief Concatenates a passed string to this string.
     *
     * @param str A character string to be appended.
     * @return True if a passed string has been appended successfully.
     */
    virtual bool_t concatenateRaw(T const* str) = 0;

    /**
     * @brief Compares this string with a passed string lexicographically.
     *
     * @param string A string object interface to be compared with.
     * @return true if this string equals to a given string.
     */
    virtual bool_t isEqualToRaw(T const* str) const = 0;
    
    /**
     * @brief Converts an integer number to a string.
     *
     * The function converts an integer value into a character string using the base parameter,
     * which has to be 2, 8, 10, or 16 based numerals for converting to an appropriate numeral system.
     *
     * @note See exceptions below:
     *
     *       Exception 1: Minimum negative value can be `T_MIN + 1` or greater.
     *
     *       Exception 2: Only if the base is decimal, a passed number is available to be negative value,
     *       and the resulting string of these values is preceded with a minus sign. 
     *       
     *       Exception 3: A hexadecimal number includes lower case characters, and any resulting strings 
     *       do not contain any suffixes or prefixes for identifying a numeral system.
     *
     * @todo Rework the implementation to avoid the exceptions.
     *
     * @param val  A value that would be converted to a string.
     * @param base A numerical base used to represent a value as a string.
     * @return True if the conversion has been completed successfully.
     */
    template <typename I>
    bool_t convertToString(I const val, Number::Base const base);

    /**
     * @brief Tests if a value is signed or unsigned.
     *
     * @param value A value that would be tested.
     * @return True if the value has been negative.
     *
     * @note The `volatile` keyword added, as the GCC 7.5.0 compiler doesn't call this function optimizing it for Release build.
     *       This behavior is cause a bug in `itoa` function with -9223372036854775808 value of int64_t type trying to check module of the value.
     *       Partial specialization of the template function for int64_t also doesn't help.
     */
    template <typename I>
    static bool_t isPositive(volatile I value);
        
    template <typename T0> friend bool_t operator==(api::String<T0> const&, T0 const* const);
    template <typename T0> friend bool_t operator==(T0 const* const, api::String<T0> const&);
    template <typename T0> friend bool_t operator!=(api::String<T0> const&, T0 const* const);
    template <typename T0> friend bool_t operator!=(T0 const* const, api::String<T0> const&);

};

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 A source object interface 1.
 * @param source2 A source object interface 2.
 * @return True if strings are equal.
 */
template <typename T>
inline bool_t operator==(api::String<T> const& source1, api::String<T> const& source2)
{
    return source1.isEqualTo(source2);
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 A source object interface 1.
 * @param source2 A source character string 2.
 * @return True if strings are equal.
 */
template <typename T>
inline bool_t operator==(api::String<T> const& source1, T const* const source2)
{
    return AbstractBaseString< T,CharTrait<T> >::isEqualRaw2(source1.getChar(), source2);
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 A source character string 1.
 * @param source2 A source object interface 2.
 * @return True if strings are equal.
 */
template <typename T>
inline bool_t operator==(T const* const source1, api::String<T> const& source2)
{
    return AbstractBaseString< T,CharTrait<T> >::isEqualRaw2(source1, source2.getChar());
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 A source object interface 1.
 * @param source2 A source object interface 2.
 * @return True if strings are not equal.
 */
template <typename T>
inline bool_t operator!=(api::String<T> const& source1, api::String<T> const& source2)
{
    return !source1.isEqualTo(source2);
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 A source object interface 1.
 * @param source2 A source character string 2.
 * @return True if strings are not  equal.
 */
template <typename T>
inline bool_t operator!=(api::String<T> const& source1, T const* const source2)
{
    return !AbstractBaseString< T,CharTrait<T> >::isEqualRaw2(source1.getChar(), source2);
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 A source character string 1.
 * @param source2 A source object interface 2.
 * @return True if strings are not equal.
 */
template <typename T>
inline bool_t operator!=(T const* const source1, api::String<T> const& source2)
{
    return !AbstractBaseString< T,CharTrait<T> >::isEqualRaw2(source1, source2.getChar());
}

template <typename T, class R, class A>
AbstractBaseString<T,R,A>::~AbstractBaseString()
{
}

template <typename T, class R, class A>
bool_t AbstractBaseString<T,R,A>::isConstructed() const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
{
    return Parent::isConstructed();
}

template <typename T, class R, class A>
bool_t AbstractBaseString<T,R,A>::isEmpty() const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
{
    bool_t res;
    size_t const length( getLength() );
    if(length == 0U)
    {
        res = true;
    }
    else
    {
        res = false;
    }
    return res;
}

template <typename T, class R, class A>
bool_t AbstractBaseString<T,R,A>::copy(api::String<T> const& string)
{
    bool_t res;
    if( isConstructed() && string.isConstructed() )
    {
        T const* const str( string.getChar() );
        res = copyRaw(str);
    }
    else
    {
        res = false;
    }
    return res;
}

template <typename T, class R, class A>
bool_t AbstractBaseString<T,R,A>::concatenate(api::String<T> const& string)
{
    bool_t res;
    if( isConstructed() && string.isConstructed() )
    {
        T const* const str( string.getChar() );
        res = concatenateRaw(str);            
    }
    else
    {
        res = false;            
    }
    return res;
}

template <typename T, class R, class A>
bool_t AbstractBaseString<T,R,A>::isEqualTo(api::String<T> const& string) const
{
    bool_t res;
    if( isConstructed() && string.isConstructed() )
    {
        T const* const str( string.getChar() );
        res = isEqualToRaw(str);   
    }
    else
    {
        res = false;            
    }
    return res;
}

template <typename T, class R, class A>
template <typename I>
bool_t AbstractBaseString<T,R,A>::convert(I const value, Number::Base const base) ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
{
    return convertToString(value, base);
} 

template <typename T, class R, class A>
AbstractBaseString<T,R,A>& AbstractBaseString<T,R,A>::operator+=(api::String<T> const& source)
{
    static_cast<void>( concatenate(source) );
    return *this;
}

template <typename T, class R, class A>
AbstractBaseString<T,R,A>& AbstractBaseString<T,R,A>::operator+=(T const* const source)
{
    static_cast<void>( concatenateRaw(source) );
    return *this;
}    

template <typename T, class R, class A>
AbstractBaseString<T,R,A>::AbstractBaseString()
    : Object<A>()
    , api::String<T>() {
}

template <typename T, class R, class A>
AbstractBaseString<T,R,A>::AbstractBaseString(AbstractBaseString const& obj)
    : Object<A>(obj)
    , api::String<T>() {
}

template <typename T, class R, class A>
AbstractBaseString<T,R,A>& AbstractBaseString<T,R,A>::operator=(AbstractBaseString const& obj)
{
    if( isConstructed() && (this != &obj) )
    {
        Parent::operator=(obj);
    }
    return *this;
}    

#if EOOS_CPP_STANDARD >= 2011

template <typename T, class R, class A>
AbstractBaseString<T,R,A>::AbstractBaseString(AbstractBaseString&& obj) noexcept 
    : Object<A>( move(obj) )
    , api::String<T>(){
}   

template <typename T, class R, class A>
AbstractBaseString<T,R,A>& AbstractBaseString<T,R,A>::operator=(AbstractBaseString&& obj) & noexcept
{
    if( isConstructed() && (this != &obj) )
    {
        Parent::operator=( move(obj) );            
    }        
    return *this;
}        

#endif // EOOS_CPP_STANDARD >= 2011

template <typename T, class R, class A>
size_t AbstractBaseString<T,R,A>::getLengthRaw(T const* str)
{
    size_t len( 0U ); 
    T const null( R::getTerminator() );
    while( *str != null )
    {
        ++str; ///< SCA MISRA-C++:2008 Justified Rule 5-0-15
        ++len;
    }
    return len;
}

template <typename T, class R, class A>
void AbstractBaseString<T,R,A>::copyRaw3(T* dst, T const* src, size_t cnt)
{
    if( (dst != NULLPTR) && (src != NULLPTR) )
    {
        T const null( R::getTerminator() );
        while(true)
        {
            if( cnt == 0U )
            {
                *dst = null;
                break;
            }     
            *dst = *src;
            --cnt;
            if( *dst == null )
            {
                break;
            }
            ++dst;
            ++src;
        }
    }
}

template <typename T, class R, class A>
void AbstractBaseString<T,R,A>::concatenateRaw3(T* dst, T const* src, size_t cnt)
{
    if( (dst != NULLPTR) && (src != NULLPTR) )
    {
        T const null( R::getTerminator() );
        while(true)
        {
            if(*dst == null)
            {
                break;
            }
            ++dst;
            --cnt;
        }
        copyRaw3(dst, src, cnt);
    }
}

template <typename T, class R, class A>
bool_t AbstractBaseString<T,R,A>::isEqualRaw2(T const* str1, T const* str2)
{
    bool_t res( false );
    if( (str1 != NULLPTR) && (str2 != NULLPTR) )
    {        
        T const null( R::getTerminator() );
        while(true)
        {
            res = *str1 == *str2;
            if( (*str1 == null) || (res == false) )
            {
                break;
            }
            ++str1; ///< SCA MISRA-C++:2008 Justified Rule 5-0-15
            ++str2; ///< SCA MISRA-C++:2008 Justified Rule 5-0-15
        }
    }
    return res;    
}

template <typename T, class R, class A>
template <typename I>
bool_t AbstractBaseString<T,R,A>::convertToString(I const val, Number::Base const base)
{
    const int32_t LENGTH( ( static_cast<int32_t>( sizeof(I) ) * 8) + 1 );
    bool_t res( true );
    bool_t isNegative( false );
    int32_t index( LENGTH - 1 );
    T temp[LENGTH];
    temp[index] = R::getTerminator();
    index -= 1;
    do
    {
        // Test for available base
        switch(base)
        {
            case Number::BASE_2:
            case Number::BASE_8:
            case Number::BASE_16:
            {
                isNegative = false;
                break;
            }
            case Number::BASE_10:
            {
                isNegative = ( !isPositive(val) ) ? true : false;
                break;
            }
            default:
            {
                res = false;
                break;
            }
        }
        // If the base is not available
        if(res == false)
        {
            break;
        }
        // Prepare absolute value
        I module( isNegative ? (0 - val) : val );
        if( !isPositive(module) )
        {
            res = false;
            break;
        }
        // Do the conversion
        // @todo Revise possibility to declare index of size_t underlying type.
        //       But in the case index will always more than or equal zero.
        //       Thus, algorithm shall be re-worked.
        while(index >= 0)
        {
            I digit( module % static_cast<I>(base) );
            temp[index] = R::convertDigitToChar(digit);
            index -= 1;
            module = module / static_cast<I>(base);
            if(module == 0)
            {
                break;
            }
        }
        // Add minus
        if( isNegative && (index >= 0) )
        {
            temp[index] = R::getMinusSign();
            index -= 1;
        }
        res = true;
    }
    while(false);
    if(res == true)
    {
        index += 1;
        res = copyRaw(&temp[index]);
    }
    return res;
}

template <typename T, class R, class A>
template <typename I>
bool_t AbstractBaseString<T,R,A>::isPositive(volatile I value)
{
    return ( (value > 0) || (value == 0) ) ? true : false;
}

} // namespace lib
} // namespace eoos
#endif // LIB_ABSTRACTBASESTRING_HPP_
