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
     * Primary template implements the static string class.
     *
     * @param Type   data type of string characters.     
     * @param LENGTH maximum number of string characters, or 0 for dynamic allocation.
     * @param Alloc  heap memory allocator class.
     */
    template <typename Type, int32 LENGTH, class Alloc = Allocator>    
    class String : public ::library::AbstractString<Type,LENGTH,Alloc>
    {
        typedef ::library::AbstractString<Type,LENGTH,Alloc> Parent;
        
        /** 
         * Constructor.
         *
         * @param string a character string to be set.         
         */    
        String(const Type* string) : Parent()
        {
        } 
        
        /**
         * Destructor.
         */
        virtual ~String()
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
     * Char type partial specialization of the static string class.
     *
     * @param Alloc heap memory allocator class.
     */
    template <int32 LENGTH, class Alloc>        
    class String<char,LENGTH,Alloc> : public ::library::AbstractString<char,LENGTH,Alloc>
    {
        typedef ::library::AbstractString<char,LENGTH,Alloc> Parent;

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
    
    #ifdef NO_STRICT_MISRA_RULES  
    
    /** 
     * Partial specialization of the template implements the dynamic string class.
     *
     * @param Type  data type of string characters.
     * @param Alloc heap memory allocator class.
     */
    template <typename Type, class Alloc>    
    class String<Type,0,Alloc> : public ::library::AbstractString<Type,0,Alloc>
    {
        typedef ::library::AbstractString<Type,0,Alloc> Parent;

    public:
    
        /** 
         * Constructor.
         *
         * @param string a character string to be set.         
         */    
        String(const Type* string) : Parent()
        {
        } 
        
        /**
         * Destructor.
         */
        virtual ~String()
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
     * Char type partial specialization of the dynamic string class.
     *
     * @param Alloc heap memory allocator class.
     */
    template <class Alloc>        
    class String<char,0,Alloc> : public ::library::AbstractString<char,0,Alloc>
    {
        typedef ::library::AbstractString<char,0,Alloc> Parent;

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

    #endif // NO_STRICT_MISRA_RULES
    
}
#endif // LIBRARY_STRING_HPP_
