/**
 * @file      lib.Memory.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_MEMORY_HPP_
#define LIB_MEMORY_HPP_

#include "Types.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class Memory
 * @brief Memory manipulator class.
 */
class Memory
{

public:

    /**
     * @brief Power of number base.
     */
    enum Base
    {
        BASE_2 = 2,
        BASE_8 = 8,
        BASE_10 = 10,
        BASE_16 = 16
    };

    /**
     * @brief Copies a block of memory.
     *
     * @param dst A destination array where the content would be copied.
     * @param src A source array to be copied.
     * @param len A number of bytes to copy.
     * @return A pointer to the destination array, or NULLPTR if an error has been occurred.
     */
    static void* memcpy(void* const dst, void const* const src, size_t len)
    {
        void* res( NULLPTR );
        if( (dst != NULLPTR) && (src != NULLPTR) )
        {
            cell_t const* sp( static_cast<cell_t const*>(src) );
            cell_t* dp( static_cast<cell_t*>(dst) );
            while(len-- != 0U) ///< SCA MISRA-C++:2008 Justified Rule 5-2-10
            {
                *dp++ = *sp++; ///< SCA MISRA-C++:2008 Justified Rule 5-0-15 and Rule 5-2-10
            }
            res = dst;
        }
        return res;
    }

    /**
     * @brief Fills a block of memory.
     *
     * @param dst A destination block of memory would be filled.
     * @param val A value to be set.
     * @param len A number of bytes to be set to the value.
     * @return A pointer to the destination memory, or NULLPTR if an error has been occurred.
     */
    static void* memset(void* const dst, cell_t const val, size_t len)
    {
        void* res( NULLPTR );
        if(dst != NULLPTR)
        {
            cell_t* dp( static_cast<cell_t*>(dst) );
            cell_t const uc( val );
            while(len-- != 0U) ///< SCA MISRA-C++:2008 Justified Rule 5-2-10
            {
                *dp++ = uc; ///< SCA MISRA-C++:2008 Justified Rule 5-0-15 and Rule 5-2-10
            }
            res = dst;
        }
        return res;
    }

    /**
     * @brief Returns the length of a passed string .
     *
     * @param str A character string would be measured.
     * @return The length of the passed string, and zero if NULLPTR given.
     */
    static size_t strlen(char_t const* str)
    {
        size_t len( 0U );        
        if(str != NULLPTR)
        {
            while( *str != '\0' )
            {
                len++;
                str++; ///< SCA MISRA-C++:2008 Justified Rule 5-0-15
            }
        }
        return len;
    }

    /**
     * @brief Copies one string to another .
     *
     * @param dst A destination array where the content would be copied.
     * @param src A character string to be copied.
     * @return A pointer to the destination string, or NULLPTR if an error has been occurred.
     */
    static char_t* strcpy(char_t* const dst, char_t const* src)
    {
        char_t* res( NULLPTR );
        if( (dst != NULLPTR) && (src != NULLPTR) )
        {
            char_t* d( dst - 1 );             ///< SCA MISRA-C++:2008 Justified Rule 5-0-15
            char_t const* s( src  - 1 );      ///< SCA MISRA-C++:2008 Justified Rule 5-0-15
            while( (*++d = *++s) != '\0' ) {} ///< SCA MISRA-C++:2008 Justified Rule 5-0-15, Rule 5-2-10 and Rule 6-2-1
            res = dst;
            
        }
        return res;
    }


    /**
     * @brief Concatenates two strings.
     *
     * @param dst A destination character string where the content would be appended.
     * @param src A character string to be appended.
     * @return A pointer to the destination string, or NULLPTR if an error has been occurred.
     */
    static char_t* strcat(char_t* const dst, char_t const* src)
    {
        char_t* res( NULLPTR );
        if( (dst != NULLPTR) && (src != NULLPTR) )
        {       
            char_t* d( dst - 1 );             ///< SCA MISRA-C++:2008 Justified Rule 5-0-15
            while( *++d != '\0' ) {}          ///< SCA MISRA-C++:2008 Justified Rule 5-0-15 and Rule 5-2-10
            d--;                              ///< SCA MISRA-C++:2008 Justified Rule 5-0-15
            char_t const* s( src - 1 );       ///< SCA MISRA-C++:2008 Justified Rule 5-0-15       
            while( (*++d = *++s) != '\0' ) {} ///< SCA MISRA-C++:2008 Justified Rule 5-0-15, Rule 5-2-10 and Rule 6-2-1
            res = dst;
        }
        return res;
    }

    /**
     * @brief Compares two strings.
     *
     * @param str1 Character string to be compared.
     * @param str2 Character string to be compared.
     * @return The value 0 if the string 1 is equal to the string 2;
     *         a value less than 0 if the first not match character of string 1 has lower value than in string 2;
     *         a value greater than 0 if the first not match character of string 1 has greater value than in string 2;     
     *         or the minimum possible value if an error has been occurred.
     */
    static int32_t strcmp(char_t const* str1, char_t const* str2)
    {
        int32_t res( static_cast<int32_t>( 0x80000000U ) );
        if( (str1 != NULLPTR) && (str2 != NULLPTR) )
        {        
            while(true)
            {
                int32_t ch1( static_cast<int32_t>(*str1++) ); ///< SCA MISRA-C++:2008 Justified Rule 5-0-15 and Rule 5-2-10
		    	int32_t ch2( static_cast<int32_t>(*str2++) ); ///< SCA MISRA-C++:2008 Justified Rule 5-0-15 and Rule 5-2-10
                res = ch1 - ch2;
                if( (ch1 == 0) || (res != 0) )
                {
                    break;
                }
            }
        }
        return res;
    }

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
     * @param str  A character string for a result of the conversion.
     * @param base A numerical base used to represent a value as a string.
     * @return True if the conversion has been completed successfully.
     */
    template <typename T>
    static bool_t itoa(T const val, char_t* str, Base const base = BASE_10)
    {
        const int32_t LENGTH( ( static_cast<int32_t>( sizeof(T) ) * 8) + 1 );
        bool_t res( false );
        if(str != NULLPTR)
        {
            char_t temp[LENGTH];
            bool_t isNegative;
            int32_t index( LENGTH - 1 );
            res = true;            
            temp[index--] = '\0'; ///< SCA MISRA-C++:2008 Justified Rule 5-0-11 and Rule 5-2-10
            do
            {
                // Test for available base
                switch(base)
                {
                    case BASE_2:
                    case BASE_8:
                    case BASE_16:
                    {
                        isNegative = false;
                        break;
                    }
                    case BASE_10:
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
                T module( isNegative ? (0 - val) : val );
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
                    char_t ch;
                    T digit( module % static_cast<T>(base) );
                    if( (base == BASE_16) && (digit > 9) )
                    {
                        ch = 'a';
                        digit -= 10;
                    }
                    else
                    {
                        ch = '0';
                    }
                    temp[index--] = static_cast<char_t>(digit + ch); ///< SCA MISRA-C++:2008 Justified Rule 3-9-2, Rule 5-0-11 and Rule 5-2-10
                    module = module / static_cast<T>(base);
                    if(module == 0)
                    {
                        break;
                    }
                }
                // Add minus
                if( isNegative && (index >= 0) )
                {
                    temp[index--] = '-'; ///< SCA MISRA-C++:2008 Justified Rule 5-0-11 and Rule 5-2-10
                }
                res = true;
            }
            while(false);
            // Copy the temp string to the destination string
            // @todo Replace this with strncpy
            strcpy(str, &temp[++index]); ///< SCA MISRA-C++:2008 Justified Rule 5-2-10
        }
        return res;
    }

    /**
     * @brief Converts a string to an integer number.
     *
     * @param str  A character string that would be converted to a number.
     * @param base A numerical base used to parse the string.
     * @return The resulting number, 0 if an error is occurred.
     */
    template <typename T>
    static T atoi(char_t const* str, Base const base = BASE_10)
    {
        bool_t isBase( false );
        switch(base)
        {
            case BASE_2:
            case BASE_8:
            case BASE_10:
            case BASE_16:
            {
                isBase = true;
                break;
            }
            default:
            {
                isBase = false;
                break;
            }
        }
        T result( 0 );        
        if( isBase )
        {
            T const multiplier( static_cast<T>(base) );
            int32_t index( 0 );
            bool_t isNegative( false );
            // Look for whitespaces
            while( isSpace(str[index]) )
            {
                index++;
            }
            // Test a character if the number is negative for decimal base
            if(base == BASE_10)
            {
                if( str[index] == '-' )
                {
                    isNegative = true;
                    index++;
                }
                else if( str[index] == '+' )
                {
                    isNegative = false;
                    index++;
                }
                else
                {
                    isNegative = false;
                }
            }
            // Do fast calculation for no hexadecimal base
            if(base != BASE_16)
            {
                while( isDigit(str[index], base) )
                {
                    result *= multiplier;
                    result += static_cast<T>( str[index++] - '0' ); ///< SCA MISRA-C++:2008 Justified Rule 5-2-10
                }
            }
            else
            {
                char_t subtrahend;
                int32_t addend;
                while( isDigit(str[index], base) )
                {
                    detectMathOperands(str[index], subtrahend, addend);
                    result *= static_cast<T>( base );
                    result += static_cast<T>( str[index++] - subtrahend ); ///< SCA MISRA-C++:2008 Justified Rule 4-5-3, Rule 5-0-11 and Rule 5-2-10
                    result += static_cast<T>( addend );
                }
            }
            result = isNegative ? (0 - result) : result;
        }
        return result;
    }

private:

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
    template <typename T>
    static bool_t isPositive(volatile T value)
    {
        return ( (value > 0) || (value == 0) ) ? true : false;
    }

    /**
     * @brief Tests if a character is a whitespace character.
     *
     * @param character A character code.
     * @return True if the character is whitespace.
     */
    static bool_t isSpace(char_t const character)
    {
        bool_t result( true );
        switch(character)
        {
            case ' ':
            case '\t':
            case '\n':
            case '\v':
            case '\f':
            case '\r':
            {
                break;
            }
            default:
            {
                result = false;
                break;
            }
        }
        return result;
    }

    /**
     * @brief Tests if a character is a decimal number.
     *
     * @param character A character code.
     * @param base      A numerical base used to parse the character.
     * @return True if the character is a decimal number.
     */
    static bool_t isDigit(char_t const character, Base const base = BASE_10)
    {
        bool_t res( false );
        int32_t const ch( static_cast<int32_t>(character) );
        switch(base)
        {
            case BASE_2:
            {
                res = ( (ch >= 0x30) && (ch <= 0x31) ) ? true : false;
                break;
            }
            case BASE_8:
            {
                res = ( (ch >= 0x30) && (ch <= 0x37) ) ? true : false;
                break;
            }
            case BASE_16:
            {
                res = ( 
                    ( (ch >= 0x30) && (ch <= 0x39) )
                 || ( (ch >= 0x41) && (ch <= 0x46) )
                 || ( (ch >= 0x61) && (ch <= 0x66) )
                ) ? true : false;
                break;
            }
            case BASE_10:
            {
                res = ( (ch >= 0x30) && (ch <= 0x39) ) ? true : false;
                break;
            }
        }
        return res;
    }

    /**
     * @brief Detects subtrahend and addend for hex numbers.
     *
     * @param ch          A testing character code.
     * @param subtrahend  A resulting subtrahend.
     * @param addend      A resulting addend.
     */
    static void detectMathOperands(char_t const character, char_t& subtrahend, int32_t& addend)
    {
        int32_t const ch( static_cast<int32_t>(character) );
        // Test for uppercase letter
        if( (ch >= 0x41) && (ch <= 0x46) )
        {
            subtrahend = 'A';
            addend = 10;
        }
        // Test for lowercase letter
        else if( (ch >= 0x61) && (ch <= 0x66) )
        {
            subtrahend = 'a';
            addend = 10;
        }
        else
        {
            subtrahend = '0';
            addend = 0;
        }
    }

};

} // namespace lib
} // namespace eoos
#endif // LIB_MEMORY_HPP_
