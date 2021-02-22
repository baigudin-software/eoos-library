/**
 * @brief Base String class.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017-2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_BASE_STRING_HPP_
#define LIB_BASE_STRING_HPP_

#include "lib.AbstractString.hpp"
#include "lib.Memory.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @brief Primary template implements the static string class.
 *
 * @tparam T - a data type of string characters.
 * @tparam L - a maximum number of string characters, or 0 for dynamic allocation.
 * @tparam A - a heap memory allocator class.
 */
template <typename T, int32_t L, class A = Allocator>
class BaseString : public AbstractString<T,L,A>
{
    typedef BaseString<T,L,A>         Self;
    typedef AbstractString<T,L,A> Parent;

    /**
     * @brief Constructor.
     */
    BaseString() : Parent()
    {
    }

    /**
     * @brief Constructor.
     *
     * @param source - a source character string.
     */
    BaseString(const T* const source) : Parent()
    {
        Parent::copy(source);
    }

    /**
     * @brief Destructor.
     */
    virtual ~BaseString()
    {
    }

protected:

    /**
     * @brief Returns this string terminated character.
     *
     * @return a character which means that this string terminated.
     */
    virtual T getTerminator() const
    {
        return T::TERMINATING_CHARACTER;
    }

};

/**
 * @brief Char type partial specialization of the static string class.
 *
 * @tparam L - a maximum number of string characters, or 0 for dynamic allocation.
 * @tparam A - a heap memory allocator class.
 */
template <int32_t L, class A>
class BaseString<char_t,L,A> : public AbstractString<char_t,L,A>
{
    typedef BaseString<char_t,L,A>           Self;
    typedef AbstractString<char_t,L,A>   Parent;

public:

    /**
     * @brief Constructor.
     */
    BaseString() : Parent()
    {
    }

    /**
     * @brief Constructor.
     *
     * @param source - a source object.
     */
    BaseString(const BaseString<char_t,L,A>& source) : Parent()
    {
        Parent::copy(source);
    }

    /**
     * @brief Constructor.
     *
     * @param source - a source object interface.
     */
    BaseString(const api::String<char_t>& source) : Parent()
    {
        Parent::copy(source);
    }

    /**
     * @brief Constructor.
     *
     * @param source - a source character string.
     */
    BaseString(const char_t* const source) : Parent()
    {
        Parent::copy(source);
    }

    /**
     * @brief Constructor.
     *
     * @param value - a source numerical value.
     * @param base - a numerical base used to represent a value as a string.
     */
    explicit BaseString(int32_t const value) : Parent()
    {
        Self::convert<int32_t>(value, 10);
    }

    /**
     * @brief Casts to int32_t type.
     *
     * @return a numerical value.
     */
    operator int32_t() const
    {
        return Self::cast<int32_t>(10);
    }

    /**
     * @brief Direct assignment operator.
     *
     * @param source - a source object.
     * @return this object.
     */
    BaseString<char_t,L,A>& operator=(const BaseString<char_t,L,A>& source)
    {
        Parent::copy(source);
        return *this;
    }

    /**
     * @brief Assignment operator.
     *
     * @param source - a source object interface.
     * @return reference to this object.
     */
    BaseString<char_t,L,A>& operator=(const api::String<char_t>& source)
    {
        Parent::copy(source);
        return *this;
    }

    /**
     * @brief Assignment operator.
     *
     * @param source - a source character string.
     * @return reference to this object.
     */
    BaseString<char_t,L,A>& operator=(const char_t* const source)
    {
        Parent::copy(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param source - a source object.
     * @return reference to this object.
     */
    BaseString<char_t,L,A>& operator+=(const BaseString<char_t,L,A>& source)
    {
        Parent::concatenate(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param source - a source object interface.
     * @return reference to this object.
     */
    BaseString<char_t,L,A>& operator+=(const api::String<char_t>& source)
    {
        Parent::concatenate(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param source - a source character string.
     * @return reference to this object.
     */
    BaseString<char_t,L,A>& operator+=(const char_t* const source)
    {
        Parent::concatenate(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param value - a source numerical value.
     * @return reference to this object.
     */
    BaseString<char_t,L,A>& operator+=(int32_t const value)
    {
        BaseString const string(value);
        Parent::concatenate(string);
        return *this;
    }

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
     * NOTE: You need to use "string.template convert<I>(value, base);" syntax,
     * if you have to specify the template argument type explicitly.
     *
     * @param val  - a value that would be converted to this string.
     * @param base - a numerical base used to represent a value as this string.
     * @return true if the conversion has been completed successfully.
     */
    template <typename I>
    bool_t convert(I const value, int32_t const base = 10)
    {
        bool_t res;
        char_t temp[sizeof(I) * 8 + 1];
        if( not Memory::itoa<I>(value, temp, base) )
        {
            res = false;
        }
        else
        {
            res = Parent::copy(temp);
        }
        return res;
    }

    /**
     * @brief Casts this string to an integer number.
     *
     * NOTE: You need to use "string.template cast<I>(base);" syntax,
     * if you have to specify the template argument type explicitly.
     *
     * @param base - a numerical base used to parse the string.
     * @return the resulting number.
     */
    template <typename I>
    I cast(int32_t const base = 10) const
    {
        return Memory::atoi<I>(Parent::getChar(), base);
    }

protected:

    /**
     * @brief Returns this string terminated character.
     *
     * @return a character which means that this string terminated.
     */
    virtual char_t getTerminator() const
    {
        return '\0';
    }

private:

    template <int32_t L0, class A0> friend bool_t operator==(const BaseString<char_t,L0,A0>&, const char_t*);
    template <int32_t L0, class A0> friend bool_t operator==(const char_t*, const BaseString<char_t,L0,A0>&);
    template <int32_t L0, class A0> friend bool_t operator!=(const BaseString<char_t,L0,A0>&, const char_t*);
    template <int32_t L0, class A0> friend bool_t operator!=(const char_t*, const BaseString<char_t,L0,A0>&);
};

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source object 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator==(const BaseString<char_t,L,A>& source1, const BaseString<char_t,L,A>& source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source object interface 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator==(const BaseString<char_t,L,A>& source1, const api::String<char_t>& source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 - a source object interface 1.
 * @param source2 - a source object 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator==(const api::String<char_t>& source1, const BaseString<char_t,L,A>& source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source character string 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator==(const BaseString<char_t,L,A>& source1, const char_t* const source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 - a source character string 1.
 * @param source2 - a source source object 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator==(const char_t* const source1, const BaseString<char_t,L,A>& source2)
{
    return ( source2.compare(source1) == 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source object 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator!=(const BaseString<char_t,L,A>& source1, const BaseString<char_t,L,A>& source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source object interface 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator!=(const BaseString<char_t,L,A>& source1, const api::String<char_t>& source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 - a source object interface 1.
 * @param source2 - a source object 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator!=(const api::String<char_t>& source1, const BaseString<char_t,L,A>& source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source character string 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator!=(const BaseString<char_t,L,A>& source1, const char_t* const source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 - a source character string 1.
 * @param source2 - a source source object 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator!=(const char_t* const source1, const BaseString<char_t,L,A>& source2)
{
    return ( source2.compare(source1) != 0 ) ? true : false;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source object 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline BaseString<char_t,L,A> operator+(const BaseString<char_t,L,A>& source1, const BaseString<char_t,L,A>& source2)
{
    BaseString<char_t,L,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source object interface 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline BaseString<char_t,L,A> operator+(const BaseString<char_t,L,A>& source1, const api::String<char_t>& source2)
{
    BaseString<char_t,L,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 - a source object interface 1.
 * @param source2 - a source object 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline BaseString<char_t,L,A> operator+(const api::String<char_t>& source1, const BaseString<char_t,L,A>& source2)
{
    BaseString<char_t,L,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source character string 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline BaseString<char_t,L,A> operator+(const BaseString<char_t,L,A>& source1, const char_t* const source2)
{
    BaseString<char_t,L,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 - a source character string 1.
 * @param source2 - a source source object 2.
 * @return true if strings are equal.
 */
template <int32_t L, class A>
inline BaseString<char_t,L,A> operator+(const char_t* const source1, const BaseString<char_t,L,A>& source2)
{
    BaseString<char_t,L,A> string(source1);
    string += source2;
    return string;
}

#ifdef EOOS_NO_STRICT_MISRA_RULES

/**
 * @brief Partial specialization of the template implements the dynamic string class.
 *
 * @tparam T - a data type of string characters.
 * @tparam A - a heap memory allocator class.
 */
template <typename T, class A>
class BaseString<T,0,A> : public AbstractString<T,0,A>
{
    typedef BaseString<T,0,A>         Self;
    typedef AbstractString<T,0,A> Parent;

public:

    /**
     * @brief Constructor.
     */
    BaseString() : Parent()
    {
    }

    /**
     * @brief Constructor.
     *
     * @param source - a source character string.
     */
    BaseString(const T* const source) : Parent()
    {
        Parent::copy(source);
    }

    /**
     * @brief Destructor.
     */
    virtual ~BaseString()
    {
    }

protected:

    /**
     * @brief Returns this string terminated character.
     *
     * @return a character which means that this string terminated.
     */
    virtual T getTerminator() const
    {
        return T::TERMINATING_CHARACTER;
    }

};

/**
 * @brief Char type partial specialization of the dynamic string class.
 *
 * @tparam A - a heap memory allocator class.
 */
template <class A>
class BaseString<char_t,0,A> : public AbstractString<char_t,0,A>
{
    typedef BaseString<char_t,0,A>         Self;
    typedef AbstractString<char_t,0,A> Parent;

public:

    /**
     * @brief Constructor.
     */
    BaseString() : Parent()
    {
    }

    /**
     * @brief Constructor.
     *
     * @param source - a source object.
     */
    BaseString(const BaseString<char_t,0,A>& source) : Parent()
    {
        Parent::copy(source);
    }

    /**
     * @brief Constructor.
     *
     * @param source - a source object interface.
     */
    BaseString(const api::String<char_t>& source) : Parent()
    {
        Parent::copy(source);
    }

    /**
     * @brief Constructor.
     *
     * @param source - a source character string.
     */
    BaseString(const char_t* const source) : Parent()
    {
        Parent::copy(source);
    }

    /**
     * @brief Constructor.
     *
     * @param value - a source numerical value.
     * @param base - a numerical base used to represent a value as a string.
     */
    explicit BaseString(int32_t const value) : Parent()
    {
        Self::convert<int32_t>(value, 10);
    }

    /**
     * @brief Casts to int32_t type.
     *
     * @return a numerical value.
     */
    operator int32_t() const
    {
        return Self::cast<int32_t>(10);
    }

    /**
     * @brief Direct assignment operator.
     *
     * @param source - a source object.
     * @return this object.
     */
    BaseString<char_t,0,A>& operator=(const BaseString<char_t,0,A>& source)
    {
        Parent::copy(source);
        return *this;
    }

    /**
     * @brief Assignment operator.
     *
     * @param source  -a source object interface.
     * @return reference to this object.
     */
    BaseString<char_t,0,A>& operator=(const api::String<char_t>& source)
    {
        Parent::copy(source);
        return *this;
    }

    /**
     * @brief Assignment operator.
     *
     * @param source - a source character string.
     * @return reference to this object.
     */
    BaseString<char_t,0,A>& operator=(const char_t* const source)
    {
        Parent::copy(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param source - a source object.
     * @return reference to this object.
     */
    BaseString<char_t,0,A>& operator+=(const BaseString<char_t,0,A>& source)
    {
        Parent::concatenate(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param source - a source object interface.
     * @return reference to this object.
     */
    BaseString<char_t,0,A>& operator+=(const api::String<char_t>& source)
    {
        Parent::concatenate(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param source - a source character string.
     * @return reference to this object.
     */
    BaseString<char_t,0,A>& operator+=(const char_t* const source)
    {
        Parent::concatenate(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param value - a source numerical value.
     * @return reference to this object.
     */
    BaseString<char_t,0,A>& operator+=(int32_t const value)
    {
        BaseString string(value);
        Parent::concatenate(string);
        return *this;
    }

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
     * NOTE: You need to use "string.template convert<I>(value, base);" syntax,
     * if you have to specify the template argument type explicitly.
     *
     * @param val  - a value that would be converted to this string.
     * @param base - a numerical base used to represent a value as this string.
     * @return true if the conversion has been completed successfully.
     */
    template <typename I>
    bool_t convert(I const value, int32_t const base = 10)
    {
        bool_t res;
        char_t temp[sizeof(I) * 8 + 1];
        if( not Memory::itoa<I>(value, temp, base) )
        {
            res = false;
        }
        else
        {
            res = Parent::copy(temp);
        }
        return res;
    }

    /**
     * @brief Casts this string to an integer number.
     *
     * NOTE: You need to use "string.template cast<I>(base);" syntax,
     * if you have to specify the template argument type explicitly.
     *
     * @param base - a numerical base used to parse the string.
     * @return the resulting number.
     */
    template <typename I>
    I cast(int32_t const base = 10) const
    {
        const char_t* const str = Parent::getChar();
        return Memory::atoi<I>(str, base);
    }

protected:

    /**
     * @brief Returns this string terminated character.
     *
     * @return a character which means that this string terminated.
     */
    virtual char_t getTerminator() const
    {
        return '\0';
    }

private:

    template <class A0> friend bool_t operator==(const BaseString<char_t,0,A0>&, const char_t*);
    template <class A0> friend bool_t operator==(const char_t*, const BaseString<char_t,0,A0>&);
    template <class A0> friend bool_t operator!=(const BaseString<char_t,0,A0>&, const char_t*);
    template <class A0> friend bool_t operator!=(const char_t*, const BaseString<char_t,0,A0>&);
};

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source object 2.
 * @return true if strings are equal.
 */
template <class A>
inline bool_t operator==(const BaseString<char_t,0,A>& source1, const BaseString<char_t,0,A>& source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source object interface 2.
 * @return true if strings are equal.
 */
template <class A>
inline bool_t operator==(const BaseString<char_t,0,A>& source1, const api::String<char_t>& source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 - a source object interface 1.
 * @param source2 - a source object 2.
 * @return true if strings are equal.
 */
template <class A>
inline bool_t operator==(const api::String<char_t>& source1, const BaseString<char_t,0,A>& source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source character string 2.
 * @return true if strings are equal.
 */
template <class A>
inline bool_t operator==(const BaseString<char_t,0,A>& source1, const char_t* const source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 - a source character string 1.
 * @param source2 - a source source object 2.
 * @return true if strings are equal.
 */
template <class A>
inline bool_t operator==(const char_t* const source1, const BaseString<char_t,0,A>& source2)
{
    return ( source2.compare(source1) == 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source object 2.
 * @return true if strings are equal.
 */
template <class A>
inline bool_t operator!=(const BaseString<char_t,0,A>& source1, const BaseString<char_t,0,A>& source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source object interface 2.
 * @return true if strings are equal.
 */
template <class A>
inline bool_t operator!=(const BaseString<char_t,0,A>& source1, const api::String<char_t>& source2)
{
    return ( source1.compare(source2) ) != 0 ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 - a source object interface 1.
 * @param source2 - a source object 2.
 * @return true if strings are equal.
 */
template <class A>
inline bool_t operator!=(const api::String<char_t>& source1, const BaseString<char_t,0,A>& source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source character string 2.
 * @return true if strings are equal.
 */
template <class A>
inline bool_t operator!=(const BaseString<char_t,0,A>& source1, const char_t* const source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 - a source character string 1.
 * @param source2 - a source source object 2.
 * @return true if strings are equal.
 */
template <class A>
inline bool_t operator!=(const char_t* const source1, const BaseString<char_t,0,A>& source2)
{
    return ( source2.compare(source1) != 0 ) ? true : false;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source object 2.
 * @return true if strings are equal.
 */
template <class A>
inline BaseString<char_t,0,A> operator+(const BaseString<char_t,0,A>& source1, const BaseString<char_t,0,A>& source2)
{
    BaseString<char_t,0,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source object interface 2.
 * @return true if strings are equal.
 */
template <class A>
inline BaseString<char_t,0,A> operator+(const BaseString<char_t,0,A>& source1, const api::String<char_t>& source2)
{
    BaseString<char_t,0,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 - a source object interface 1.
 * @param source2 - a source object 2.
 * @return true if strings are equal.
 */
template <class A>
inline BaseString<char_t,0,A> operator+(const api::String<char_t>& source1, const BaseString<char_t,0,A>& source2)
{
    BaseString<char_t,0,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 - a source object 1.
 * @param source2 - a source character string 2.
 * @return true if strings are equal.
 */
template <class A>
inline BaseString<char_t,0,A> operator+(const BaseString<char_t,0,A>& source1, const char_t* const source2)
{
    BaseString<char_t,0,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 - a source character string 1.
 * @param source2 - a source source object 2.
 * @return true if strings are equal.
 */
template <class A>
inline BaseString<char_t,0,A> operator+(const char_t* const source1, const BaseString<char_t,0,A>& source2)
{
    BaseString<char_t,0,A> string(source1);
    string += source2;
    return string;
}

#endif // EOOS_NO_STRICT_MISRA_RULES

} // namespace lib
} // namespace eoos
#endif // LIB_BASE_STRING_HPP_
