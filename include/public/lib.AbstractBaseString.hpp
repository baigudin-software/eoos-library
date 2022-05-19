/**
 * @file      lib.AbstractBaseString.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ABSTRACTBASESTRING_HPP_
#define LIB_ABSTRACTBASESTRING_HPP_

#include "lib.Object.hpp"
#include "api.String.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class AbstractBaseString<T,A>
 * @brief Abstract base string class.
 *
 * @tparam T A data type of string characters.
 * @tparam A A heap memory allocator class.
 */
template <typename T, class A = Allocator>
class AbstractBaseString : public Object<A>, public api::String<T>
{
    typedef AbstractBaseString<T,A>  Self;
    typedef Object<A> Parent;

public:

    /**
     * @brief Constructor.
     */
    AbstractBaseString() 
        : Object<A>()
        , api::String<T>() {
    }

    /**
     * @brief Destructor.
     */
    virtual ~AbstractBaseString()
    {
    }

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        return Parent::isConstructed();
    }

    /**
     * @copydoc eoos::api::Collection::isEmpty()
     */
    virtual bool_t isEmpty() const
    {
        bool_t res;
        int32_t const length( getLength() );
        if(length == 0)
        {
            res = true;
        }
        else
        {
            res = false;
        }
        return res;
    }

    /**
     * @copydoc eoos::api::String::copy(const api::String<T>&)
     */
    virtual bool_t copy(api::String<T> const& string)
    {
        bool_t res;
        if( (!Self::isConstructed()) || (!string.isConstructed()) )
        {
            res = false;
        }
        else
        {
            T const* const str( string.getChar() );
            res = copy(str);
        }
        return res;
    }

    /**
     * @copydoc eoos::api::String::concatenate(const api::String<T>&)
     */
    virtual bool_t concatenate(api::String<T> const& string)
    {
        bool_t res;
        if( (!Self::isConstructed()) || (!string.isConstructed()) )
        {
            res = false;
        }
        else
        {
            T const* const str( string.getChar() );
            res = concatenate(str);
        }
        return res;
    }

    /**
     * @copydoc eoos::api::String::compare(const api::String<T>&)
     */
    virtual int32_t compare(api::String<T> const& string) const
    {
        int32_t res;
        if( (!Self::isConstructed()) || (!string.isConstructed()) )
        {
            res = MINIMUM_POSSIBLE_VALUE_OF_INT32;
        }
        else
        {
            T const* const str( string.getChar() );
            res = compare(str);
        }
        return res;
    }
    
    /**
     * @copydoc eoos::api::Collection::getLength()
     * @note Declared to declare the parent function
     */
    virtual int32_t getLength() const = 0; 

protected:

    /**
     * @brief Returns this string terminated character.
     *
     * @return A character which means that this string terminated.
     */
    virtual T getTerminator() const = 0;

    /**
     * @brief Copies a passed string into this string.
     *
     * @param str A character string to be copied.
     * @return True if a passed string has been copied successfully.
     */
    virtual bool_t copy(T const* str) = 0;

    /**
     * @brief Concatenates a passed string to this string.
     *
     * @param str A character string to be appended.
     * @return True if a passed string has been appended successfully.
     */
    virtual bool_t concatenate(T const* str) = 0;

    /**
     * @brief Compares this string with a passed string lexicographically.
     *
     * @param str A character string to be compared.
     * @return The value 0 if a passed string is equal to this string;
     *         a value less than 0 if this string is less than a passed string;
     *         a value greater than 0 if this string is greater than a passed string,
     *         or the minimum possible value if an error has been occurred.
     */
    virtual int32_t compare(T const* str) const = 0;

    /**
     * @brief Returns a string length.
     *
     * @param str A character string would be measured.
     * @return A length of the passed string.
     */
    int32_t getLength(T const* str) const
    {
        int32_t len( 0 ); 
        T const null( getTerminator() );
        while( *str != null )
        {
            str++; ///< SCA MISRA-C++:2008 Justified Rule 5-0-15
            len++;
        }
        return len;
    }

    /**
     * @brief Copies a string.
     *
     * @param dst A destination array where the content would be copied.
     * @param src A character string to be copied.
     */
    void copy(T* dst, T const* src) const
    {
        if( (dst != NULLPTR) && (src != NULLPTR) )
        {
            T const null( getTerminator() );
            dst--; ///< SCA MISRA-C++:2008 Justified Rule 5-0-15
            src--; ///< SCA MISRA-C++:2008 Justified Rule 5-0-15
            do
            {
                dst++; ///< SCA MISRA-C++:2008 Justified Rule 5-0-15
                src++; ///< SCA MISRA-C++:2008 Justified Rule 5-0-15
                *dst = *src;
            }
            while(*dst != null);
        }
    }

    /**
     * @brief Concatenates two strings.
     *
     * @param dst A destination character string where the content would be appended.
     * @param src An appended character string.
     */
    void concatenate(T* const dst, T const* const src) const
    {
        if( (dst != NULLPTR) && (src != NULLPTR) )
        {
            T const null( getTerminator() );
            int32_t d( 0 );
            int32_t s( 0 );
            while( dst[d] != null )
            {
                d++;
            }
            while(true)
            {
                dst[d] = src[s]; 
                if(dst[d] == null)
                {
                    break;
                }
                d++;
                s++;
            }
        }
    }

    /**
     * @brief The minimum possible value of int32_t type.
     */
    static const int32_t MINIMUM_POSSIBLE_VALUE_OF_INT32 = 0 - 0x7fffffff - 1 ;

private:

    /**
     * @brief Constructor.
     *
     * Creating of the object copy is accessible
     * by creating the default object and
     * calling the copy interface function.
     *
     * @param obj A source object.
     */
    AbstractBaseString(AbstractBaseString<T,A> const& obj);

};

} // namespace lib
} // namespace eoos
#endif // LIB_ABSTRACTBASESTRING_HPP_
