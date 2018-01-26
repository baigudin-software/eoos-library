/** 
 * Abstract base string class.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017-2018, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_ABSTRACT_BASE_STRING_HPP_
#define LIBRARY_ABSTRACT_BASE_STRING_HPP_

#include "library.Object.hpp"
#include "api.String.hpp"

namespace library
{
    /**
     * Primary template implementation.
     *
     * @param Type  data type of string characters.
     * @param Alloc heap memory allocator class.
     */
    template <typename Type, class Alloc = Allocator>    
    class AbstractBaseString : public ::library::Object<Alloc>, public ::api::String<Type>
    {
        typedef ::library::Object<Alloc> Parent;

    public:
    
        /** 
         * Constructor.
         */    
        AbstractBaseString() : Parent()
        {
        } 
        
        /**
         * Destructor.
         */
        virtual ~AbstractBaseString()
        {
        }   
        
        /**
         * Returns a number of elements in this container.
         *
         * @return number of elements.
         */
        virtual int32 getLength() const = 0;  
        
        /**
         * Returns pointer to the first character of containing string.
         *
         * @return first character of containing string characters, or NULL if no string contained.
         */
        virtual const Type* getChar() const = 0;                      
    
        /**
         * Tests if this object has been constructed.
         *
         * @return true if object has been constructed successfully.
         */    
        virtual bool isConstructed() const
        {
            return this->isConstructed_;
        }
        
        /**
         * Tests if this collection has elements.
         *
         * @return true if this collection does not contain any elements.
         */
        virtual bool isEmpty() const
        {
            return getLength() == 0 ? true : false;
        }
        
        /** 
         * Copies a passed string into this string.
         *
         * @param string a string object to be copied.         
         * @return true if a passed string has been copied successfully.
         */
        virtual bool copy(const ::api::String<Type>& string)
        {
            if( not this->isConstructed_ || not string.isConstructed() )
            {
                return false;
            }
            const Type* str = string.getChar();
            return copy(str);
        }        

        /** 
         * Concatenates a passed string to this string.             
         *
         * @param string a string object to be appended.
         * @return true if a passed string has been appended successfully.          
         */
        virtual bool concatenate(const ::api::String<Type>& string)
        {
            if( not this->isConstructed_ || not string.isConstructed() )
            {
                return false;
            }
            const Type* str = string.getChar();
            return concatenate(str);
        } 
        
        /** 
         * Compares this string with a passed string lexicographically.         
         *
         * @param string a string object to be compared.
         * @return the value 0 if a passed string is equal to this string; 
         *         a value less than 0 if this string is less than a passed string; 
         *         a value greater than 0 if this string is greater than a passed string,
         *         or the minimum possible value if an error has been occurred.
         */
        virtual int32 compare(const ::api::String<Type>& string) const
        {
            if( not this->isConstructed_ || not string.isConstructed() )
            {
                return MINIMUM_POSSIBLE_VALUE_OF_INT32;
            }
            const Type* str = string.getChar();
            return compare(str);            
        }
        
    protected:
    
        /**
         * Returns this string terminated character.
         *
         * @return a character which means that this string terminated.
         */         
        virtual Type getTerminator() const = 0;    

        /** 
         * Copies a passed string into this string.
         *
         * @param str a character string to be copied.
         * @return true if a passed string has been copied successfully.
         */
        virtual bool copy(const Type* str) = 0;             

        /** 
         * Concatenates a passed string to this string.             
         *
         * @param str an character string to be appended.             
         * @return true if a passed string has been appended successfully.          
         */
        virtual bool concatenate(const Type* str) = 0;

        /** 
         * Compares this string with a passed string lexicographically.         
         *
         * @param str a character string to be compared.
         * @return the value 0 if a passed string is equal to this string; 
         *         a value less than 0 if this string is less than a passed string; 
         *         a value greater than 0 if this string is greater than a passed string,
         *         or the minimum possible value if an error has been occurred.         
         */
        virtual int32 compare(const Type* str) const = 0;    

        /** 
         * Returns a string length.
         *
         * @param str a character string would be measured.
         * @return a length of the passed string.
         */
        int32 getLength(const Type* str) const
        {
            if(str == NULL) 
            {
                return 0;
            }
            Type null = getTerminator();            
            int32 l = 0;
            while( str[l] != null ) 
            {
                l++;
            }
            return l;
        }
        
        /** 
         * Copies a string.
         *
         * @param dst a destination array where the content would be copied.
         * @param src character string to be copied.
         */
        void copy(Type* dst, const Type* src) const
        {
            if(dst == NULL || src == NULL) 
            {
                return;
            }
            Type null = getTerminator();
            int32 i = 0;
            while( (dst[i] = src[i]) != null )
            {
                i++;
            }
        }

        /** 
         * Concatenates two strings.
         *
         * @param dst a destination character string where the content would be appended.
         * @param src an appended character string.
         */
        void concatenate(Type* dst, const Type* src) const
        {
            if(dst == NULL || src == NULL) 
            {
                return;
            }
            Type null = getTerminator();
            int32 d = 0;
            int32 s = 0;            
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
        
        /**
         * The minimum possible value of int32 type.
         */
        static const int32 MINIMUM_POSSIBLE_VALUE_OF_INT32 = 0x80000000;        
        
    private:
    
        /** 
         * Constructor.
         *
         * Creating of the object copy is accessible 
         * by creating the default object and 
         * calling the copy interface function.
         *
         * @param obj a source object.
         */
        AbstractBaseString(const AbstractBaseString<Type,Alloc>& obj);            
        
    };
}
#endif // LIBRARY_ABSTRACT_BASE_STRING_HPP_
