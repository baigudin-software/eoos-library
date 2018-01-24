/** 
 * String class.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_STRING_HPP_
#define LIBRARY_STRING_HPP_

#include "library.AbstractString.hpp"

namespace library
{
    /** 
     * Primary template.
     *
     * @param Type  data type of string characters.
     * @param Alloc heap memory allocator class.
     */
    template <typename Type, class Alloc = Allocator>    
    class String : public ::library::AbstractString<Type,Alloc>
    {
        typedef ::library::AbstractString<Type,Alloc> Parent;

    public:
    
        /** 
         * Constructor.
         *
         * @param string a character string to be set.         
         */    
        String(const Type* string) : Parent()
        {
        }     
    
    protected:

        /**
         * Returns this string terminated character.
         *
         * @return a character which means that this string terminated.
         */         
        virtual Type getTerminator() const
        {
            return Type::TERMINATING_CHARACTER;
        }
    
    };
    
    /** 
     * Partial specialization of the primary template for char type.
     *
     * @param Type  data type of buffer element.     
     * @param Alloc heap memory allocator class.
     */
    template <class Alloc>
    class String<char, Alloc> : public ::library::AbstractString<char,Alloc>
    {
        typedef ::library::AbstractString<char, Alloc> Parent;

    public:
    
        /** 
         * Constructor.
         *
         * @param string a character string to be set.         
         */    
        String(const char* string) : Parent()
        {
            this->copy(string);
        }     
    
    protected:

        /**
         * Returns this string terminated character.
         *
         * @return a character which means that this string terminated.
         */         
        virtual char getTerminator() const
        {
            return '\0';
        }    
    };
}
#endif // LIBRARY_STRING_HPP_
