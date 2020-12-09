/**
 * @brief Abstract base string class.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017-2020, Sergey Baigudin, Baigudin Software
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
 * @brief Primary template implementation.
 *
 * @param T - a data type of string characters.
 * @param A - a heap memory allocator class.
 */
template <typename T, class A = Allocator>
class AbstractBaseString : public Object<A>, public api::String<T>
{
    typedef AbstractBaseString<T,A>  Self;
    typedef ::eoos::lib::Object<A> Parent;

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
     * @brief Returns a number of elements in this container.
     *
     * @return number of elements.
     */
    virtual int32_t getLength() const = 0;

    /**
     * @brief Returns pointer to the first character of containing string.
     *
     * @return first character of containing string characters, or NULLPTR if no string contained.
     */
    virtual const T* getChar() const = 0;

    /**
     * @brief Tests if this object has been constructed.
     *
     * @return true if object has been constructed successfully.
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }

    /**
     * @brief Tests if this collection has elements.
     *
     * @return true if this collection does not contain any elements.
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
     * @brief Copies a passed string into this string.
     *
     * @param string - a string object to be copied.
     * @return true if a passed string has been copied successfully.
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
     * @brief Concatenates a passed string to this string.
     *
     * @param string - a string object to be appended.
     * @return true if a passed string has been appended successfully.
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
     * @brief Compares this string with a passed string lexicographically.
     *
     * @param string - a string object to be compared.
     * @return the value 0 if a passed string is equal to this string;
     *         a value less than 0 if this string is less than a passed string;
     *         a value greater than 0 if this string is greater than a passed string,
     *         or the minimum possible value if an error has been occurred.
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

protected:

    /**
     * @brief Returns this string terminated character.
     *
     * @return a character which means that this string terminated.
     */
    virtual T getTerminator() const = 0;

    /**
     * @brief Copies a passed string into this string.
     *
     * @param str - a character string to be copied.
     * @return true if a passed string has been copied successfully.
     */
    virtual bool_t copy(const T* str) = 0;

    /**
     * @brief Concatenates a passed string to this string.
     *
     * @param str - a character string to be appended.
     * @return true if a passed string has been appended successfully.
     */
    virtual bool_t concatenate(const T* str) = 0;

    /**
     * @brief Compares this string with a passed string lexicographically.
     *
     * @param str - a character string to be compared.
     * @return the value 0 if a passed string is equal to this string;
     *         a value less than 0 if this string is less than a passed string;
     *         a value greater than 0 if this string is greater than a passed string,
     *         or the minimum possible value if an error has been occurred.
     */
    virtual int32_t compare(const T* str) const = 0;

    /**
     * @brief Returns a string length.
     *
     * @param str - a character string would be measured.
     * @return a length of the passed string.
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
     * @param dst - a destination array where the content would be copied.
     * @param src - a character string to be copied.
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
     * @param dst - a destination character string where the content would be appended.
     * @param src - an appended character string.
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
     * @param obj - a source object.
     */
    AbstractBaseString(const AbstractBaseString<T,A>& obj);

};

} // namespace lib
} // namespace eoos
#endif // LIB_ABSTRACT_BASE_STRING_HPP_
