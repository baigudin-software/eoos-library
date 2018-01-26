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
         */    
        String() : Parent()
        {
        }
        
        /** 
         * Constructor.
         *
         * @param source a source character string.         
         */    
        String(const Type* source) : Parent()
        {
            this->copy(source);        
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
         */    
        String() : Parent()
        {
        }    
    
        /** 
         * Constructor.
         *
         * @param source a source character string.         
         */    
        String(const char* source) : Parent()
        {
            this->copy(source);
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
         */    
        String() : Parent()
        {
        }    
    
        /** 
         * Constructor.
         *
         * @param source a source character string.        
         */    
        String(const Type* source) : Parent()
        {
            this->copy(source);        
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
         */    
        String() : Parent()
        {
        }
        
        /** 
         * Constructor.
         *
         * @param source a source object.         
         */             
        String(const ::library::String<char,0,Alloc>& source) : Parent()       
        {
            this->copy(source);        
        }    
        
        /** 
         * Constructor.
         *
         * @param source a source object interface.
         */             
        String(const ::api::String<char>& source) : Parent()       
        {
            this->copy(source);        
        }                  
             
        /** 
         * Constructor.
         *
         * @param source a source character string.
         */    
        String(const char* source) : Parent()
        {
            this->copy(source);
        }
        
        /**
         * Direct assignment operator.
         *
         * @param source a source object.
         * @return this object.     
         */
        ::library::String<char,0,Alloc>& operator =(const ::library::String<char,0,Alloc>& source)
        {
            this->copy(source);
            return *this;            
        }
        
        /** 
         * Assignment operator.
         *
         * @param source a source object interface.
         * @return reference to this object.       
         */     
        ::library::String<char,0,Alloc>& operator =(const ::api::String<char>& source)
        {
            this->copy(source);        
            return *this;        
        }   
        
        /** 
         * Assignment operator.
         *
         * @param source a source character string.
         * @return reference to this object.       
         */     
        ::library::String<char,0,Alloc>& operator =(const char* source)
        {
            this->copy(source);        
            return *this;        
        }             
        
        /** 
         * Assignment by sum operator.
         *
         * @param source a source object.
         * @return reference to this object.       
         */     
        ::library::String<char,0,Alloc>& operator +=(const ::library::String<char,0,Alloc>& source)
        {
            this->concatenate(source);
            return *this;   
        }
        
        /** 
         * Assignment by sum operator.
         *
         * @param source a source object interface.
         * @return reference to this object.       
         */     
        ::library::String<char,0,Alloc>& operator +=(const ::api::String<char>& source)
        {
            this->concatenate(source);
            return *this;   
        }  
        
        /** 
         * Assignment by sum operator.
         *
         * @param source a source character string.
         * @return reference to this object.       
         */     
        ::library::String<char,0,Alloc>& operator +=(const char* source)
        {
            this->concatenate(source);
            return *this;   
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
