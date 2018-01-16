/** 
 * Class of static methods to manipulate memory.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2018, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_MEMORY_HPP_
#define LIBRARY_MEMORY_HPP_

#include "Types.hpp"

namespace library
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
         * @return a pointer to the destination array, or NULL if an error has been occurred.         
         */
        static void* memcpy(void* dst, const void* src, size_t len)
        {
            if(dst == NULL || src == NULL) 
            {
                return NULL;
            }         
            cell* sp  = static_cast<cell*>(const_cast<void*>(src));
            cell* dp  = static_cast<cell*>(dst);
            while(len--) *dp++ = *sp++;
            return dst;
        }
        
        /** 
         * Fills a block of memory.
         *
         * @param dst a destination block of memory would be filled.
         * @param val a value to be set.
         * @param len a number of bytes to be set to the value.
         * @return a pointer to the destination memory, or NULL if an error has been occurred.         
         */
        static void* memset(void* dst, cell val, size_t len)
        {
            if(dst == NULL) 
            {
                return NULL;
            }         
            cell* dp = static_cast<cell*>(dst);    
            const cell uc = val;
            while(len--) *dp++ = uc;
            return dst;
        }
        
        /** 
         * Gets a string length.
         *
         * @param str a C-string would be measured.
         * @return a length of the passed string.
         */
        static size_t strlen(const char* str)
        {
            if(str == NULL)
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
         * Copies a string.
         *
         * @param dst a destination array where the content would be copied.
         * @param src C-string to be copied.
         * @return a pointer to the destination string, or NULL if an error has been occurred.
         */
        static char* strcpy(char* dst, const char* src)
        {
            if(dst == NULL || src == NULL) 
            {
                return NULL;
            }        
            char* d = dst - 1;     
            const char* s = src  - 1;     
            while( (*++d = *++s) != '\0' );
            return dst;
        }
        
        
        /** 
         * Concatenates strings.
         *
         * @param dst a destination C-string where the content would be appended.
         * @param src an appended C-string.
         * @return a pointer to the destination string, or NULL if an error has been occurred.
         */
        static char* strcat(char* dst, const char* src)
        {
            if(dst == NULL || src == NULL) 
            {
                return NULL;
            }        
            char* d = dst - 1;
            const char* s = src - 1;
            while( *++d );
            d--;
            while( (*++d = *++s) != '\0' );
            return dst;
        }
        
        /** 
         * Compares two strings.
         *
         * @param str1 C-string to be compared.
         * @param str2 C-string to be compared.
         * @return the value 0 if the string 1 is equal to the string 2; 
         *         a value less than 0 if the string 1 is shorter than the string 2; 
         *         a value greater than 0 if the string 1 is longer than the string 2, 
         *         or the maximum positive value if an error has been occurred.         
         */
        static int32 strcmp(const char* str1, const char* str2)
        {
            if(str1 == NULL || str2 == NULL) 
            {
                return 0x7FFFFFFF;
            }        
            int32 ch, res;
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
         * Converts integer number to string.
         *
         * @param val  a value that would be converted to a string.
         * @param str  a resulting C-string.
         * @param base a numerical base used to represent the value as a string.
         * @return a pointer to the passed string, or NULL if an error has been occurred.
         */        
        static char* itoa(int32 val, char* str, int32 base = 10)
        {
            bool isNegative = val < 0;        
            if(str == NULL)
            {
                return NULL;
            }
            switch(base)
            {
                case  8:
                case 16: 
                {
                    if(isNegative)
                    {
                        return NULL;
                    }
                    break;
                }
                case 10:                                
                {
                    break;
                }
                default: 
                {
                    return NULL;
                }
            }         
            static const int32 LENGTH = 16;
            char temp[LENGTH];
            int32 module = isNegative ? 0 - val : val;            
            int32 i = LENGTH - 1;
            temp[i--] = '\0';
            do
            { 
                char ch;
                int32 digit = module % base;
                if(base == 16 && digit > 9)
                {
                    ch = 'a';
                    digit -= 10;
                }
                else
                {
                    ch = '0';                
                }
                temp[i--] = static_cast<char>(digit + ch);
                module = module / base;
            } 
            while(module != 0);
            if(isNegative) 
            {
                temp[i--] = '-';
            }
            strcpy(str, &temp[++i]);
            return str;
        }        
  
    };
}
#endif // LIBRARY_MEMORY_HPP_

