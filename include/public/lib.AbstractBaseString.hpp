/**
 * @file      lib.AbstractBaseString.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017-2021, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ABSTRACT_BASE_STRING_HPP_
#define LIB_ABSTRACT_BASE_STRING_HPP_

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
    AbstractBaseString() : Parent()
    {
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
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }

    /**
     * @copydoc eoos::api::Collection::isEmpty()
     */
    virtual bool_t isEmpty() const
    {
        bool_t res;
        int32_t const length = getLength();
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
    virtual bool_t copy(const api::String<T>& string)
    {
        bool_t res;
        if( not Self::isConstructed() || not string.isConstructed() )
        {
            res = false;
        }
        else
        {
            const T* const str = string.getChar();
            res = copy(str);
        }
        return res;
    }

    /**
     * @copydoc eoos::api::String::concatenate(const api::String<T>&)
     */
    virtual bool_t concatenate(const api::String<T>& string)
    {
        bool_t res;
        if( not Self::isConstructed() || not string.isConstructed() )
        {
            res = false;
        }
        else
        {
            const T* const str = string.getChar();
            res = concatenate(str);
        }
        return res;
    }

    /**
     * @copydoc eoos::api::String::compare(const api::String<T>&)
     */
    virtual int32_t compare(const api::String<T>& string) const
    {
        int32_t res;
        if( not Self::isConstructed() || not string.isConstructed() )
        {
            res = MINIMUM_POSSIBLE_VALUE_OF_INT32;
        }
        else
        {
            const T* const str = string.getChar();
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
    virtual bool_t copy(const T* str) = 0;

    /**
     * @brief Concatenates a passed string to this string.
     *
     * @param str A character string to be appended.
     * @return True if a passed string has been appended successfully.
     */
    virtual bool_t concatenate(const T* str) = 0;

    /**
     * @brief Compares this string with a passed string lexicographically.
     *
     * @param str A character string to be compared.
     * @return The value 0 if a passed string is equal to this string;
     *         a value less than 0 if this string is less than a passed string;
     *         a value greater than 0 if this string is greater than a passed string,
     *         or the minimum possible value if an error has been occurred.
     */
    virtual int32_t compare(const T* str) const = 0;

    /**
     * @brief Returns a string length.
     *
     * @param str A character string would be measured.
     * @return A length of the passed string.
     */
    int32_t getLength(const T* str) const
    {
        int32_t len = 0;
        T const null = getTerminator();
        while( *str != null )
        {
            str++;
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
    void copy(T* dst, const T* src) const
    {
        if(dst != NULLPTR && src != NULLPTR)
        {
            T const null = getTerminator();
            dst--;
            src--;
            do
            {
                dst++;
                src++;
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
    void concatenate(T* const dst, const T* const src) const
    {
        if(dst != NULLPTR && src != NULLPTR)
        {
            T const null = getTerminator();
            int32_t d = 0;
            int32_t s = 0;
            while( dst[d] != null )
            {
                d++;
            }
            while( (dst[d] = src[s]) != null )
            {
                d++;
                s++;
            }
        }
    }

    /**
     * @brief The minimum possible value of int32_t type.
     */
    static const int32_t MINIMUM_POSSIBLE_VALUE_OF_INT32 = 0 - 0x7fffffff - 1;

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
    AbstractBaseString(const AbstractBaseString<T,A>& obj);

};

} // namespace lib
} // namespace eoos
#endif // LIB_ABSTRACT_BASE_STRING_HPP_
