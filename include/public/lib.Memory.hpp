/**
 * Class of static methods to manipulate memory.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_MEMORY_HPP_
#define LIB_MEMORY_HPP_

#include "Types.hpp"

namespace eoos
{
    namespace lib
    {
        class Memory
        {

        public:

            /**
             * Copies a block of memory.
             *
             * @param dst a destination array where the content would be copied.
             * @param src a source array to be copied.
             * @param len a number of bytes to copy.
             * @return a pointer to the destination array, or NULLPTR if an error has been occurred.
             */
            static void* memcpy(void* const dst, const void* src, size_t len)
            {
                void* res;
                if(dst != NULLPTR && src != NULLPTR)
                {
                    cell_t* sp  = static_cast<cell_t*>(const_cast<void*>(src));
                    cell_t* dp  = static_cast<cell_t*>(dst);
                    while(len--)
                    {
                        *dp++ = *sp++;
                    }
                    res = dst;
                }
                else
                {
                    res = NULLPTR;
                }
                return res;
            }

            /**
             * Fills a block of memory.
             *
             * @param dst a destination block of memory would be filled.
             * @param val a value to be set.
             * @param len a number of bytes to be set to the value.
             * @return a pointer to the destination memory, or NULLPTR if an error has been occurred.
             */
            static void* memset(void* const dst, const cell_t val, size_t len)
            {
                if(dst == NULLPTR)
                {
                    return NULLPTR;
                }
                cell_t* dp = static_cast<cell_t*>(dst);
                const cell_t uc = val;
                while(len--) *dp++ = uc;
                return dst;
            }

            /**
             * Returns the length of a passed string .
             *
             * @param str a character string would be measured.
             * @return the length of the passed string.
             */
            static size_t strlen(const char_t* str)
            {
                if(str == NULLPTR)
                {
                    return 0;
                }
                size_t len = 0;
                while( *str != '\0' )
                {
                    len++;
                    str++;
                }
                return len;
            }

            /**
             * Copies one string to another .
             *
             * @param dst a destination array where the content would be copied.
             * @param src a character string to be copied.
             * @return a pointer to the destination string, or NULLPTR if an error has been occurred.
             */
            static char_t* strcpy(char_t* const dst, const char_t* src)
            {
                if(dst == NULLPTR || src == NULLPTR)
                {
                    return NULLPTR;
                }
                char_t* d = dst - 1;
                const char_t* s = src  - 1;
                while( (*++d = *++s) != '\0' );
                return dst;
            }


            /**
             * Concatenates two strings.
             *
             * @param dst a destination character string where the content would be appended.
             * @param src a character string to be appended.
             * @return a pointer to the destination string, or NULLPTR if an error has been occurred.
             */
            static char_t* strcat(char_t* const dst, const char_t* src)
            {
                if(dst == NULLPTR || src == NULLPTR)
                {
                    return NULLPTR;
                }
                char_t* d = dst - 1;
                const char_t* s = src - 1;
                while( *++d );
                d--;
                while( (*++d = *++s) != '\0' );
                return dst;
            }

            /**
             * Compares two strings.
             *
             * @param str1 character string to be compared.
             * @param str2 character string to be compared.
             * @return the value 0 if the string 1 is equal to the string 2;
             *         a value less than 0 if the string 1 is shorter than the string 2;
             *         a value greater than 0 if the string 1 is longer than the string 2,
             *         or the minimum possible value if an error has been occurred.
             */
            static int32_t strcmp(const char_t* str1, const char_t* str2)
            {
                if(str1 == NULLPTR || str2 == NULLPTR)
                {
                    return 0x80000000;
                }
                int32_t ch, res;
                while(true)
                {
                    ch = *str1++;
                    res = ch - *str2++;
                    if(ch == 0 || res != 0)
                    {
                        break;
                    }
                }
                return res;
            }

            /**
             * Converts an integer number to a string.
             *
             * The function converts an integer value into a character string using the base parameter,
             * which has to be 2, 8, 10, or 16 based numerals for converting to an appropriate numeral system.
             *
             * Mark that only if the base is decimal, a passed number is available to be negative values,
             * and the resulting string of these values is preceded with a minus sign. In addition,
             * a hexadecimal number includes lower case characters, and any resulting strings do not contain
             * any suffixes or prefixes for identifying a numeral system.
             *
             * @param val  a value that would be converted to a string.
             * @param str  a character string for a result of the conversion.
             * @param base a numerical base used to represent a value as a string.
             * @return true if the conversion has been completed successfully.
             */
            template <typename T>
            static bool_t itoa(const T val, char* str, const int32_t base = 10)
            {
                const int32_t LENGTH = sizeof(T) * 8 + 1;
                if(str == NULLPTR)
                {
                    return false;
                }
                char temp[LENGTH];
                bool_t isNegative;
                bool_t res = true;
                int32_t index = LENGTH - 1;
                temp[index--] = '\0';
                do
                {
                    // Test for available base
                    switch(base)
                    {
                        case  2:
                        case  8:
                        case 16:
                        {
                            isNegative = false;
                            break;
                        }
                        case 10:
                        {
                            isNegative = not isPositive(val) ? true : false;
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
                    T module = isNegative ? 0 - val : val;
                    if( not isPositive(module) )
                    {
                        res = false;
                        break;
                    }
                    // Do the conversion
                    while(index >= 0)
                    {
                        char ch;
                        T digit = module % base;
                        if(base == 16 && digit > 9)
                        {
                            ch = 'a';
                            digit -= 10;
                        }
                        else
                        {
                            ch = '0';
                        }
                        temp[index--] = static_cast<char>(digit + ch);
                        module = module / base;
                        if(module == 0)
                        {
                            break;
                        }
                    }
                    // Add minus
                    if(isNegative && index >= 0)
                    {
                        temp[index--] = '-';
                    }
                    res = true;
                }
                while(false);
                // Copy the temp string to the destination string
                strcpy(str, &temp[++index]);
                return res;
            }

            /**
             * Converts a string to an integer number.
             *
             * @param str a character string that would be converted to a number.
             * @param base a numerical base used to parse the string.
             * @return the resulting number.
             */
            template <typename T>
            static T atoi(const char* str, const int32_t base = 10)
            {
                switch(base)
                {
                    case  2:
                    case  8:
                    case 10:
                    case 16: break;
                    default: return 0;
                }

                T result = 0;
                const T multiplier = static_cast<T>(base);
                int32_t index = 0;
                bool_t isNegative = false;
                // Look for whitespaces
                while( isSpace(str[index]) )
                {
                    index++;
                }
                // Test a character if the number is negative for decimal base
                if(base == 10)
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
                }
                // Do fast calculation for no hexadecimal base
                if(base != 16)
                {
                    while( isDigit(str[index], base) )
                    {
                        result *= multiplier;
                        result += static_cast<T>( str[index++] - '0' );
                    }
                }
                else
                {
                    char subtrahend;
                    int32_t addend;
                    while( isDigit(str[index], base) )
                    {
                        detectMathOperands(str[index], subtrahend, addend);
                        result *= base;
                        result += static_cast<T>( str[index++] - subtrahend );
                        result += static_cast<T>( addend );
                    }
                }

                return isNegative ? 0 - result : result;
            }

        private:

            /**
             * Test if a value is signed or unsigned.
             *
             * @param value a value that would be tested.
             * @return true if the value has been negative.
             */
            template <typename T>
            static bool_t isPositive(const T value)
            {
                return value > 0 || value == 0 ? true : false;
            }

            /**
             * Tests if a character is a whitespace character.
             *
             * @param ch a character code.
             * @return true if the character is whitespace.
             */
            static bool_t isSpace(const int32_t ch)
            {
                return ch == 0x20 || (ch >= 0x09 && ch <= 0x0D) ? true : false;
            }

            /**
             * Tests if a character is a decimal number.
             *
             * @param ch a character code.
             * @param base a numerical base used to parse the character.
             * @return true if the character is a decimal number.
             */
            static bool_t isDigit(const int32_t ch, const int32_t base = 10)
            {
                switch(base)
                {
                    case 2:
                        return ch >= 0x30 && ch <= 0x31 ? true : false;

                    case 8:
                        return ch >= 0x30 && ch <= 0x37 ? true : false;

                    case 16:
                        return ch >= 0x30 && ch <= 0x39
                            || ch >= 0x41 && ch <= 0x46
                            || ch >= 0x61 && ch <= 0x66 ? true : false;

                    case 10:
                        return ch >= 0x30 && ch <= 0x39 ? true : false;

                    default:
                        return false;
                }

            }

            /**
             * Detect subtrahend and addend for hex numbers.
             *
             * @param testCh    a testing character code.
             * @param subCh     a resulting subtrahend.
             * @param subDecade a resulting addend.
             */
            static void detectMathOperands(const int32_t testCh, char& subtrahend, int32_t& addend)
            {
                // Test for uppercase letter
                if(testCh >= 0x41 && testCh <= 0x46)
                {
                    subtrahend = 'A';
                    addend = 10;
                }
                // Test for lowercase letter
                else if(testCh >= 0x61 && testCh <= 0x66)
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
    }
}
#endif // LIB_MEMORY_HPP_
