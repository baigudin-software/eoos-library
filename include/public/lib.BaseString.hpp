/**
 * @file      lib.BaseString.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_BASESTRING_HPP_
#define LIB_BASESTRING_HPP_

#include "lib.AbstractString.hpp"
#include "lib.Memory.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class BaseString<T,L,A>
 * @brief Static base string class.
 *
 * Primary template implements the static string class.
 *
 * @tparam T A data type of string characters.
 * @tparam L A maximum number of string characters, or 0 for dynamic allocation.
 * @tparam A A heap memory allocator class.
 */
template <typename T, int32_t L, class A = Allocator>
class BaseString : public AbstractString<T,L,A>
{
    typedef BaseString<T,L,A>         Self;
    typedef AbstractString<T,L,A> Parent;

    /**
     * @brief Constructor.
     */
    BaseString() 
        : AbstractString<T,L,A>() {
    }

    /**
     * @brief Constructor.
     *
     * @param source A source character string.
     */
    BaseString(T const* const source) 
        : AbstractString<T,L,A>() {
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
     * @copydoc eoos::lib::AbstractBaseString::getTerminator() const
     */
    virtual T getTerminator() const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        return T::TERMINATING_CHARACTER;
    }

};

/**
 * @class BaseString <L,A>
 * @brief Static char base string class.
 *
 * Char type partial specialization of the static string class.
 *
 * @tparam L A maximum number of string characters, or 0 for dynamic allocation.
 * @tparam A A heap memory allocator class.
 */
template <int32_t L, class A>
class BaseString<char_t,L,A> : public AbstractString<char_t,L,A>
{
    typedef BaseString<char_t,L,A>     Self;
    typedef AbstractString<char_t,L,A> Parent;

public:

    /**
     * @brief Constructor.
     */
    BaseString() 
        : AbstractString<char_t,L,A>() {
    }

    /**
     * @brief Constructor.
     *
     * @param source A source object.
     */
    BaseString(BaseString<char_t,L,A> const& source) ///< SCA MISRA-C++:2008 Justified Rule 12-8-1
        : AbstractString<char_t,L,A>() {
        Parent::copy(source);
    }

    /**
     * @brief Constructor.
     *
     * @param source A source object interface.
     */
    BaseString(api::String<char_t> const& source) 
        : AbstractString<char_t,L,A>() {
        Parent::copy(source);
    }

    /**
     * @brief Constructor.
     *
     * @param source A source character string.
     */
    BaseString(char_t const* const source) 
        : AbstractString<char_t,L,A>() {
        Parent::copy(source);
    }

    /**
     * @brief Constructor.
     *
     * @param value A source numerical value.
     * @param base  A numerical base used to represent a value as a string.
     */
    explicit BaseString(int32_t const value) 
        : AbstractString<char_t,L,A>() {
        static_cast<void>( Self::convert<int32_t>(value, 10) );
    }

    /**
     * @brief Casts to int32_t type.
     *
     * @return A numerical value.
     */
    operator int32_t() const
    {
        return Self::cast<int32_t>(10);
    }

    /**
     * @brief Direct assignment operator.
     *
     * @param source A source object.
     * @return This object.
     */
    BaseString<char_t,L,A>& operator=(BaseString<char_t,L,A> const& source)
    {
        Parent::copy(source);
        return *this;
    }

    /**
     * @brief Assignment operator.
     *
     * @param source A source object interface.
     * @return Reference to this object.
     */
    BaseString<char_t,L,A>& operator=(api::String<char_t> const& source)
    {
        Parent::copy(source);
        return *this;
    }

    /**
     * @brief Assignment operator.
     *
     * @param source A source character string.
     * @return Reference to this object.
     */
    BaseString<char_t,L,A>& operator=(char_t const* const source)
    {
        Parent::copy(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param source A source object.
     * @return Reference to this object.
     */
    BaseString<char_t,L,A>& operator+=(BaseString<char_t,L,A> const& source)
    {
        Parent::concatenate(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param source A source object interface.
     * @return Reference to this object.
     */
    BaseString<char_t,L,A>& operator+=(api::String<char_t> const& source)
    {
        Parent::concatenate(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param source A source character string.
     * @return Reference to this object.
     */
    BaseString<char_t,L,A>& operator+=(char_t const* const source)
    {
        Parent::concatenate(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param value A source numerical value.
     * @return Reference to this object.
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
     * @note You need to use "string.template convert<I>(value, base);" syntax,
     * if you have to specify the template argument type explicitly.
     *
     * @param val  A value that would be converted to this string.
     * @param base A numerical base used to represent a value as this string.
     * @return True if the conversion has been completed successfully.
     */
    template <typename I>
    bool_t convert(I const value, int32_t const base = 10) ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        bool_t res;
        char_t temp[ (sizeof(I) * 8U) + 1U ];
        if( !Memory::itoa<I>(value, temp, base) )
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
     * @note You need to use "BaseString.template cast<I>(base);" syntax,
     * if you have to specify the template argument type explicitly.
     *
     * @param base A numerical base used to parse the string.
     * @return The resulting number.
     */
    template <typename I>
    I cast(int32_t const base = 10) const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        return Memory::atoi<I>(Parent::getChar(), base);
    }

protected:

    /**
     * @copydoc eoos::lib::AbstractBaseString::getTerminator() const
     */
    virtual char_t getTerminator() const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        return '\0';
    }

private:

    template <int32_t L0, class A0> friend bool_t operator==(BaseString<char_t,L0,A0> const&, char_t const*);
    template <int32_t L0, class A0> friend bool_t operator==(char_t* const, BaseString<char_t,L0,A0> const&);
    template <int32_t L0, class A0> friend bool_t operator!=(BaseString<char_t,L0,A0> const&, char_t const*);
    template <int32_t L0, class A0> friend bool_t operator!=(char_t const*, BaseString<char_t,L0,A0> const&);
};

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator==(BaseString<char_t,L,A> const& source1, BaseString<char_t,L,A> const& source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object interface 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator==(BaseString<char_t,L,A> const& source1, api::String<char_t> const& source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 A source object interface 1.
 * @param source2 A source object 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator==(api::String<char_t> const& source1, BaseString<char_t,L,A> const& source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source character string 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator==(BaseString<char_t,L,A> const& source1, char_t const* const source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 A source character string 1.
 * @param source2 A source source object 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator==(char_t const* const source1, BaseString<char_t,L,A> const& source2)
{
    return ( source2.compare(source1) == 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator!=(BaseString<char_t,L,A> const& source1, BaseString<char_t,L,A> const& source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object interface 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator!=(BaseString<char_t,L,A> const& source1, api::String<char_t> const& source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 A source object interface 1.
 * @param source2 A source object 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator!=(api::String<char_t> const& source1, BaseString<char_t,L,A> const& source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source character string 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator!=(BaseString<char_t,L,A> const& source1, char_t const* const source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 A source character string 1.
 * @param source2 A source source object 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline bool_t operator!=(char_t const* const source1, BaseString<char_t,L,A> const& source2)
{
    return ( source2.compare(source1) != 0 ) ? true : false;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline BaseString<char_t,L,A> operator+(BaseString<char_t,L,A> const& source1, BaseString<char_t,L,A> const& source2)
{
    BaseString<char_t,L,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object interface 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline BaseString<char_t,L,A> operator+(BaseString<char_t,L,A> const& source1, api::String<char_t> const& source2)
{
    BaseString<char_t,L,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source object interface 1.
 * @param source2 A source object 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline BaseString<char_t,L,A> operator+(api::String<char_t> const& source1, BaseString<char_t,L,A> const& source2)
{
    BaseString<char_t,L,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source character string 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline BaseString<char_t,L,A> operator+(BaseString<char_t,L,A> const& source1, char_t const* const source2)
{
    BaseString<char_t,L,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source character string 1.
 * @param source2 A source source object 2.
 * @return True if strings are equal.
 */
template <int32_t L, class A>
inline BaseString<char_t,L,A> operator+(char_t const* const source1, BaseString<char_t,L,A> const& source2)
{
    BaseString<char_t,L,A> string(source1);
    string += source2;
    return string;
}

#ifdef EOOS_NO_STRICT_MISRA_RULES

/**
 * @class BaseString<T,A>
 * @brief Dynamic base string class.
 *
 * Partial specialization of the template implements the dynamic string class.
 *
 * @tparam T A data type of string characters.
 * @tparam A A heap memory allocator class.
 */
template <typename T, class A>
class BaseString<T,0,A> : public AbstractString<T,0,A>
{
    typedef BaseString<T,0,A>     Self;
    typedef AbstractString<T,0,A> Parent;

public:

    /**
     * @brief Constructor.
     */
    BaseString() 
        : AbstractString<T,0,A>() {
    }

    /**
     * @brief Constructor.
     *
     * @param source A source character string.
     */
    BaseString(T const* const source) 
        : AbstractString<T,0,A>() {
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
     * @copydoc eoos::lib::AbstractBaseString::getTerminator() const
     */
    virtual T getTerminator() const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        return T::TERMINATING_CHARACTER;
    }

};

/**
 * @class BaseString<A>
 * @brief Dynamic base string class.
 * 
 * Char type partial specialization of the dynamic string class.
 *
 * @tparam A A heap memory allocator class.
 */
template <class A>
class BaseString<char_t,0,A> : public AbstractString<char_t,0,A>
{
    typedef BaseString<char_t,0,A>     Self;
    typedef AbstractString<char_t,0,A> Parent;

public:

    /**
     * @brief Power of number base.
     */
    enum Base
    {
        BASE_2 = Memory::BASE_2,
        BASE_8 = Memory::BASE_8,
        BASE_10 = Memory::BASE_10,
        BASE_16 = Memory::BASE_16
    };    

    /**
     * @brief Constructor.
     */
    BaseString() 
        : AbstractString<char_t,0,A>() {
    }

    /**
     * @brief Constructor.
     *
     * @param source A source object.
     */
    BaseString(BaseString<char_t,0,A> const& source) ///< SCA MISRA-C++:2008 Justified Rule 12-8-1
        : AbstractString<char_t,0,A>() {
        Parent::copy(source);
    }

    /**
     * @brief Constructor.
     *
     * @param source A source object interface.
     */
    BaseString(api::String<char_t> const& source) 
        : AbstractString<char_t,0,A>() {
        Parent::copy(source);
    }

    /**
     * @brief Constructor.
     *
     * @param source A source character string.
     */
    BaseString(char_t const* const source) 
        : AbstractString<char_t,0,A>() {
        Parent::copy(source);
    }

    /**
     * @brief Constructor.
     *
     * @param value A source numerical value.
     * @param base  A numerical base used to represent a value as a string.
     */
    explicit BaseString(int32_t const value) 
        : AbstractString<char_t,0,A>() {
        static_cast<void>( Self::convert<int32_t>(value, 10) );
    }

    /**
     * @brief Casts to int32_t type.
     *
     * @return A numerical value.
     */
    operator int32_t() const
    {
        return Self::cast<int32_t>(10);
    }

    /**
     * @brief Direct assignment operator.
     *
     * @param source A source object.
     * @return This object.
     */
    BaseString<char_t,0,A>& operator=(BaseString<char_t,0,A> const& source)
    {
        Parent::copy(source);
        return *this;
    }

    /**
     * @brief Assignment operator.
     *
     * @param source A source object interface.
     * @return Reference to this object.
     */
    BaseString<char_t,0,A>& operator=(api::String<char_t> const& source)
    {
        Parent::copy(source);
        return *this;
    }

    /**
     * @brief Assignment operator.
     *
     * @param source A source character string.
     * @return Reference to this object.
     */
    BaseString<char_t,0,A>& operator=(char_t const* const source)
    {
        Parent::copy(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param source A source object.
     * @return Reference to this object.
     */
    BaseString<char_t,0,A>& operator+=(BaseString<char_t,0,A> const& source)
    {
        Parent::concatenate(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param source A source object interface.
     * @return Reference to this object.
     */
    BaseString<char_t,0,A>& operator+=(api::String<char_t> const& source)
    {
        Parent::concatenate(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param source A source character string.
     * @return Reference to this object.
     */
    BaseString<char_t,0,A>& operator+=(char_t const* const source)
    {
        Parent::concatenate(source);
        return *this;
    }

    /**
     * @brief Assignment by sum operator.
     *
     * @param value A source numerical value.
     * @return Reference to this object.
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
     * @note You need to use "string.template convert<I>(value, base);" syntax,
     * if you have to specify the template argument type explicitly.
     *
     * @param value A value that would be converted to this string.
     * @param base  A numerical base used to represent a value as this string.
     * @return True if the conversion has been completed successfully.
     */
    template <typename I>
    bool_t convert(I const value, Base const base = BASE_10) ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        bool_t res;
        char_t temp[ (sizeof(I) * 8U) + 1U ];
        if( !Memory::itoa<I>(value, temp, static_cast<Memory::Base>(base)) )
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
     * @note You need to use "BaseString.template cast<I>(base);" syntax,
     * if you have to specify the template argument type explicitly.
     *
     * @param base A numerical base used to parse the string.
     * @return The resulting number.
     */
    template <typename I>
    I cast(Base const base = BASE_10) const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        char_t const* const str( Parent::getChar() );
        return Memory::atoi<I>(str, static_cast<Memory::Base>(base));
    }

protected:

    /**
     * @copydoc eoos::lib::AbstractBaseString::getTerminator() const
     */
    virtual char_t getTerminator() const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        return '\0';
    }

private:

    template <class A0> friend bool_t operator==(BaseString<char_t,0,A0> const&, char_t const*);
    template <class A0> friend bool_t operator==(char_t const*, BaseString<char_t,0,A0> const&);
    template <class A0> friend bool_t operator!=(BaseString<char_t,0,A0> const&, char_t const*);
    template <class A0> friend bool_t operator!=(char_t const*, BaseString<char_t,0,A0> const&);
};

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object 2.
 * @return True if strings are equal.
 */
template <class A>
inline bool_t operator==(BaseString<char_t,0,A> const& source1, BaseString<char_t,0,A> const& source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object interface 2.
 * @return True if strings are equal.
 */
template <class A>
inline bool_t operator==(BaseString<char_t,0,A> const& source1, api::String<char_t> const& source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 A source object interface 1.
 * @param source2 A source object 2.
 * @return True if strings are equal.
 */
template <class A>
inline bool_t operator==(api::String<char_t> const& source1, BaseString<char_t,0,A> const& source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source character string 2.
 * @return True if strings are equal.
 */
template <class A>
inline bool_t operator==(BaseString<char_t,0,A> const& source1, char_t const* const source2)
{
    return ( source1.compare(source2) == 0 ) ? true : false;
}

/**
 * @brief Compares for equality of two strings.
 *
 * @param source1 A source character string 1.
 * @param source2 A source source object 2.
 * @return True if strings are equal.
 */
template <class A>
inline bool_t operator==(char_t const* const source1, BaseString<char_t,0,A> const& source2)
{
    return ( source2.compare(source1) == 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object 2.
 * @return True if strings are equal.
 */
template <class A>
inline bool_t operator!=(BaseString<char_t,0,A> const& source1, BaseString<char_t,0,A> const& source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object interface 2.
 * @return True if strings are equal.
 */
template <class A>
inline bool_t operator!=(BaseString<char_t,0,A> const& source1, api::String<char_t> const& source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 A source object interface 1.
 * @param source2 A source object 2.
 * @return True if strings are equal.
 */
template <class A>
inline bool_t operator!=(api::String<char_t> const& source1, BaseString<char_t,0,A> const& source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source character string 2.
 * @return True if strings are equal.
 */
template <class A>
inline bool_t operator!=(BaseString<char_t,0,A> const& source1, char_t const* const source2)
{
    return ( source1.compare(source2) != 0 ) ? true : false;
}

/**
 * @brief Compares for inequality of two strings.
 *
 * @param source1 A source character string 1.
 * @param source2 A source source object 2.
 * @return True if strings are equal.
 */
template <class A>
inline bool_t operator!=(char_t const* const source1, BaseString<char_t,0,A> const& source2)
{
    return ( source2.compare(source1) != 0 ) ? true : false;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object 2.
 * @return True if strings are equal.
 */
template <class A>
inline BaseString<char_t,0,A> operator+(BaseString<char_t,0,A> const& source1, BaseString<char_t,0,A> const& source2)
{
    BaseString<char_t,0,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object interface 2.
 * @return True if strings are equal.
 */
template <class A>
inline BaseString<char_t,0,A> operator+(BaseString<char_t,0,A> const& source1, api::String<char_t> const& source2)
{
    BaseString<char_t,0,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source object interface 1.
 * @param source2 A source object 2.
 * @return True if strings are equal.
 */
template <class A>
inline BaseString<char_t,0,A> operator+(api::String<char_t> const& source1, BaseString<char_t,0,A> const& source2)
{
    BaseString<char_t,0,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source character string 2.
 * @return True if strings are equal.
 */
template <class A>
inline BaseString<char_t,0,A> operator+(BaseString<char_t,0,A> const& source1, char_t const* const source2)
{
    BaseString<char_t,0,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source character string 1.
 * @param source2 A source source object 2.
 * @return True if strings are equal.
 */
template <class A>
inline BaseString<char_t,0,A> operator+(char_t const* const source1, BaseString<char_t,0,A> const& source2)
{
    BaseString<char_t,0,A> string(source1);
    string += source2;
    return string;
}

#endif // EOOS_NO_STRICT_MISRA_RULES

} // namespace lib
} // namespace eoos
#endif // LIB_BASESTRING_HPP_
