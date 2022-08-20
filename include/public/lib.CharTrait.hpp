/**
 * @file      lib.CharTrait.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_CHARTRAIT_HPP_
#define LIB_CHARTRAIT_HPP_

#include "Types.hpp"

namespace eoos
{
namespace lib
{

/**
 * @struct CharTrait<T>
 * @brief Abstract basic character operations.
 *
 * Has explicit template specializations for char_t, wchar_t, char16_t, char32_t, char8_t types.
 *
 * @tparam T A data type of string characters.
 */
template <typename T>
struct CharTrait
{
    /**
     * @brief Returns the terminating null character.
     *
     * @return A character means a null-terminated string is terminated.
     */
    static T getTerminator();

    /**
     * @brief Converts an integer to a character.
     *
     * @param val  A value that would be converted to a character.
     * @return Converted character as integer.
     *
     * @tparam I An integer data type to be converted.
     */    
    template <typename I>
    static T convertDigitToChar(I val);

    /**
     * @brief Returns minus sign.
     *
     * @return Minus sign.
     */
    static T getMinusSign();
};


/**
 * @struct CharTrait<char_t>
 * @brief Ordinary character literal.
 */
template <>
struct CharTrait<char_t>
{
    /**
     * @copydoc eoos::lib::CharTrait<T>::getTerminator()
     */
    static char_t getTerminator()
    {
        return '\0';
    }

    /**
     * @copydoc eoos::lib::CharTrait<T>::convertDigitToChar(I)
     */    
    template <typename I>
    static char_t convertDigitToChar(I val)
    {
        char_t ch( ' ' );
        switch(val)
        {
            case 0x00: { ch = '0'; break;}
            case 0x01: { ch = '1'; break;}
            case 0x02: { ch = '2'; break;}
            case 0x03: { ch = '3'; break;}
            case 0x04: { ch = '4'; break;}
            case 0x05: { ch = '5'; break;}
            case 0x06: { ch = '6'; break;}
            case 0x07: { ch = '7'; break;}
            case 0x08: { ch = '8'; break;}
            case 0x09: { ch = '9'; break;}
            case 0x0A: { ch = 'a'; break;}
            case 0x0B: { ch = 'b'; break;}
            case 0x0C: { ch = 'c'; break;}
            case 0x0D: { ch = 'd'; break;}
            case 0x0E: { ch = 'e'; break;}
            case 0x0F: { ch = 'f'; break;}
            default:   { ch = ' '; break;}
        }
        return ch;
    }

    /**
     * @copydoc eoos::lib::CharTrait<T>::getMinusSign()
     */
    static char_t getMinusSign()
    {
        return '-';
    }
};

/**
 * @struct CharTrait<wchar_t>
 * @brief Wide character literal.
 */
template <>
struct CharTrait<wchar_t>
{
    /**
     * @copydoc eoos::lib::CharTrait<T>::getTerminator()
     */
    static wchar_t getTerminator()
    {
        return L'\0';
    }

    /**
     * @copydoc eoos::lib::CharTrait<T>::convertDigitToChar(I)
     */    
    template <typename I>
    static wchar_t convertDigitToChar(I val)
    {
        char_t ch( L' ' );
        switch(val)
        {
            case 0x00: { ch = L'0'; break;}
            case 0x01: { ch = L'1'; break;}
            case 0x02: { ch = L'2'; break;}
            case 0x03: { ch = L'3'; break;}
            case 0x04: { ch = L'4'; break;}
            case 0x05: { ch = L'5'; break;}
            case 0x06: { ch = L'6'; break;}
            case 0x07: { ch = L'7'; break;}
            case 0x08: { ch = L'8'; break;}
            case 0x09: { ch = L'9'; break;}
            case 0x0A: { ch = L'a'; break;}
            case 0x0B: { ch = L'b'; break;}
            case 0x0C: { ch = L'c'; break;}
            case 0x0D: { ch = L'd'; break;}
            case 0x0E: { ch = L'e'; break;}
            case 0x0F: { ch = L'f'; break;}
            default:   { ch = L' '; break;}
        }
        return ch;
    }

    /**
     * @copydoc eoos::lib::CharTrait<T>::getMinusSign()
     */
    static wchar_t getMinusSign()
    {
        return L'-';
    }    
};

#if EOOS_CPP_STANDARD >= 2011

/**
 * @struct CharTrait<char16_t>
 * @brief UTF-16 character literal.
 */
template <>
struct CharTrait<char16_t>
{
    /**
     * @copydoc eoos::lib::CharTrait<T>::getTerminator()
     */
    static char16_t getTerminator()
    {
        return u'\0';
    }

    /**
     * @copydoc eoos::lib::CharTrait<T>::convertDigitToChar(I)
     */    
    template <typename I>
    static char16_t convertDigitToChar(I val)
    {
        char_t ch( u' ' );
        switch(val)
        {
            case 0x00: { ch = u'0'; break;}
            case 0x01: { ch = u'1'; break;}
            case 0x02: { ch = u'2'; break;}
            case 0x03: { ch = u'3'; break;}
            case 0x04: { ch = u'4'; break;}
            case 0x05: { ch = u'5'; break;}
            case 0x06: { ch = u'6'; break;}
            case 0x07: { ch = u'7'; break;}
            case 0x08: { ch = u'8'; break;}
            case 0x09: { ch = u'9'; break;}
            case 0x0A: { ch = u'a'; break;}
            case 0x0B: { ch = u'b'; break;}
            case 0x0C: { ch = u'c'; break;}
            case 0x0D: { ch = u'd'; break;}
            case 0x0E: { ch = u'e'; break;}
            case 0x0F: { ch = u'f'; break;}
            default:   { ch = u' '; break;}
        }
        return ch;
    }

    /**
     * @copydoc eoos::lib::CharTrait<T>::getMinusSign()
     */
    static char16_t getMinusSign()
    {
        return u'-';
    }    
};

/**
 * @struct CharTrait<char32_t>
 * @brief UTF-32 character literal.
 */
template <>
struct CharTrait<char32_t>
{
    /**
     * @copydoc eoos::lib::CharTrait<T>::getTerminator()
     */
    static char32_t getTerminator()
    {
        return U'\0';
    }

    /**
     * @copydoc eoos::lib::CharTrait<T>::convertDigitToChar(I)
     */    
    template <typename I>
    static char32_t convertDigitToChar(I val)
    {
        char_t ch( U' ' );
        switch(val)
        {
            case 0x00: { ch = U'0'; break;}
            case 0x01: { ch = U'1'; break;}
            case 0x02: { ch = U'2'; break;}
            case 0x03: { ch = U'3'; break;}
            case 0x04: { ch = U'4'; break;}
            case 0x05: { ch = U'5'; break;}
            case 0x06: { ch = U'6'; break;}
            case 0x07: { ch = U'7'; break;}
            case 0x08: { ch = U'8'; break;}
            case 0x09: { ch = U'9'; break;}
            case 0x0A: { ch = U'a'; break;}
            case 0x0B: { ch = U'b'; break;}
            case 0x0C: { ch = U'c'; break;}
            case 0x0D: { ch = U'd'; break;}
            case 0x0E: { ch = U'e'; break;}
            case 0x0F: { ch = U'f'; break;}
            default:   { ch = U' '; break;}
        }
        return ch;
    }

    /**
     * @copydoc eoos::lib::CharTrait<T>::getMinusSign()
     */
    static char32_t getMinusSign()
    {
        return U'-';
    }    
};

#endif // EOOS_CPP_STANDARD >= 2011

#if EOOS_CPP_STANDARD >= 2020

/**
 * @struct CharTrait<char8_t>
 * @brief UTF-8 character literal.
 */
template <>
struct CharTrait<char8_t>
{
    /**
     * @copydoc eoos::lib::CharTrait<T>::getTerminator()
     */
    static char8_t getTerminator()
    {
        return u8'\0';
    }

    /**
     * @copydoc eoos::lib::CharTrait<T>::convertDigitToChar(I)
     */    
    template <typename I>
    static char8_t convertDigitToChar(I val)
    {
        char_t ch( u8' ' );
        switch(val)
        {
            case 0x00: { ch = u8'0'; break;}
            case 0x01: { ch = u8'1'; break;}
            case 0x02: { ch = u8'2'; break;}
            case 0x03: { ch = u8'3'; break;}
            case 0x04: { ch = u8'4'; break;}
            case 0x05: { ch = u8'5'; break;}
            case 0x06: { ch = u8'6'; break;}
            case 0x07: { ch = u8'7'; break;}
            case 0x08: { ch = u8'8'; break;}
            case 0x09: { ch = u8'9'; break;}
            case 0x0A: { ch = u8'a'; break;}
            case 0x0B: { ch = u8'b'; break;}
            case 0x0C: { ch = u8'c'; break;}
            case 0x0D: { ch = u8'd'; break;}
            case 0x0E: { ch = u8'e'; break;}
            case 0x0F: { ch = u8'f'; break;}
            default:   { ch = u8' '; break;}
        }
        return ch;
    }

    /**
     * @copydoc eoos::lib::CharTrait<T>::getMinusSign()
     */
    static char8_t getMinusSign()
    {
        return u8'-';
    }    
};

#endif // EOOS_CPP_STANDARD >= 2020

} // namespace lib
} // namespace eoos
#endif // LIB_CHARTRAIT_HPP_
