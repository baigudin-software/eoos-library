/** 
 * Abstract string class.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017-2018, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_ABSTRACT_STRING_HPP_
#define LIBRARY_ABSTRACT_STRING_HPP_

#include "library.Object.hpp"
#include "api.String.hpp"
#include "library.Buffer.hpp"

namespace library
{
    /**
     * @param Type  data type of string characters.
     * @param Alloc heap memory allocator class.
     */
    template <typename Type, class Alloc = Allocator>    
    class AbstractString : public ::library::Object<Alloc>, public ::api::String<Type>
    {
        typedef ::library::Object<Alloc> Parent;
        struct Contex;

    public:
    
        /** 
         * Constructor.
         */    
        AbstractString() : Parent(),
            context_ (){
        }     
        
        /**
         * Destructor.
         */
        virtual ~AbstractString()
        {
            deleteString();
        }           
    
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
         * Returns a number of elements in this container.
         *
         * @return number of elements.
         */
        virtual int32 getLength() const
        {
            return this->isConstructed_ ? context_.len : 0;
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
            // TODO
            const Type* str = string.getChar();
            bool res = copy( str );
            this->setConstruct( res );
            return res;            
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
            bool res = concatenate( str );
            this->setConstruct( res );
            return res;  
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
            return compare( string.getChar() );
        }
        
        /**
         * Returns pointer to the first character of containing string.
         *
         * NOTE: Be careful, some action with the object might relocate internal buffer 
         * that contains characters. By this reason, a returned address will be actual 
         * until you do not call no constant method of this class for an object.
         *
         * @return first character of containing string characters, or NULL if no string contained.
         */
        virtual const Type* getChar() const
        {
            return context_.str;
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
        bool copy(const Type* str)
        {
            if( not this->isConstructed_ )
            {
                return false;
            }        
            if(str == NULL) 
            {
                return false;            
            }           
            int32 len = getLength(str);
            // If a given string length is more than this max available length
            if( not isFit(len) ) 
            {
                // Delete this string and its context
                deleteString();
                // Create a new temporary string and its context
                Context context;
                context.len = len;
                context.str = createString(context.len, context.max);
                if(context.str == NULL)
                {
                    return false;                
                }
                context_ = context;
            }
            else
            {
                context_.len = len;
            }
            copy(context_.str, str);                                    
            return true;        
        }               
        
        /** 
         * Concatenates a passed string to this string.             
         *
         * @param str an character string to be appended.             
         * @return true if a passed string has been appended successfully.          
         */
        bool concatenate(const Type* str)
        {
            if( not this->isConstructed_ )
            {
                return false;
            }                
            if(context_.str == NULL || str == NULL) 
            {
                return false;            
            }
            int32 len, max;
            len = getLength(str) + context_.len;          
            if( not isFit(len) ) 
            {
                Type* tmp = createString(len, max);
                if(tmp != NULL)
                {
                    copy(tmp, context_.str);
                }
                deleteString();
                context_.str = tmp;
                context_.max = max;                    
            }
            if(context_.str == NULL) 
            {
                return false;                
            }
            context_.len = len;
            concatenate(context_.str, str);
            return true;        
        }
        
        /** 
         * Compares this string with a passed string lexicographically.         
         *
         * @param str a character string to be compared.
         * @return the value 0 if a passed string is equal to this string; 
         *         a value less than 0 if this string is less than a passed string; 
         *         a value greater than 0 if this string is greater than a passed string,
         *         or the minimum possible value if an error has been occurred.         
         */
        int32 compare(const Type* str) const
        {
            if( not this->isConstructed_ )
            {
                return MINIMUM_POSSIBLE_VALUE_OF_INT32;
            }                
            if(context_.str == NULL || str == NULL) 
            {
                return MINIMUM_POSSIBLE_VALUE_OF_INT32;            
            }        
            int32 val[2];
            int32 res = context_.len - getLength(str);
            if(res != 0) 
            {
                return res;
            }
            for(int32 i=0; i<context_.len; i++)
            {
                val[0] = context_.str[i];
                val[1] = str[i];
                res = val[0] - val[1];
                if(res != 0) 
                {
                    break;
                }
            }
            return res;        
        }        
             
    private:
        
        /** 
         * Tests if a passed length fits to this data buffer.
         *
         * @param len a number of string characters.
         * @return true if this length will be fit successfully.
         */        
        bool isFit(int32 len) const
        {
            return len > context_.max ? false : true;
        }        
        
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
         * Creates a new data buffer for string.
         *
         * @param len a number of string characters.
         * @param max a setting number to bytes of allocated array.
         * @return a new array for character string.
         */        
        Type* createString(int32 len, int32& max) const
        {
            // Calculate size in byte for the given length
            int32 size = calculateSize(len);
            // Allocate a new array
            Type* data = reinterpret_cast<Type*>( Alloc::allocate(size) );
            // Calculate the max length
            max = data != NULL ? calculateLength(size) : 0;
            return data;
        }        
        
        /** 
         * Deletes an allocated character string.
         *
         * @param str a character string would be deleted and set to NULL.
         * @param max a setting number to zero.
         */        
        void deleteString()
        {
            if(context_.str != NULL) 
            {
                Alloc::free(context_.str);
            }
            context_.str = NULL;
            context_.len = 0;            
            context_.max = 0;
        }            
        
        /** 
         * Returns size in byte for a string length.
         *
         * @param len a number of string characters.
         * @return size in byte for a passed string.
         */
        static int32 calculateSize(int32 len)
        {
            size_t size = static_cast<size_t>(len) * sizeof(Type) + sizeof(Type);
            // Align size to eight
            if(size & 0x7) 
            {
                size = (size & ~0x7) + 0x8;
            }
            return static_cast<int32>(size);
        }
        
        /** 
         * Returns a string length of size in byte.
         *
         * @param size size in byte.
         * @return a number of string characters.
         */
        static int32 calculateLength(int32 size)
        {
            int32 charSize =  static_cast<int32>( sizeof(Type) );
            if(charSize == 0) 
            {
                return 0;
            }
            int32 len = size / charSize;
            return len > 1 ? len - 1 : 0;
        }        
        
        /**
         * The minimum possible value of int32 type.
         */
        static const int32 MINIMUM_POSSIBLE_VALUE_OF_INT32 = 0x80000000;                           

        /**
         * A contex of this class containing string.
         */        
        struct Context
        {
            /** 
             * Constructor.
             */
            Context() :
                str (NULL),
                len (0),
                max (0){
            }
            
            /** 
             * Destructor.
             */
           ~Context()
            {
            }            
        
            /**
             * The first character of this string.
             */        
            Type* str;
            
            /**
             * Current number of characters of this string.
             */                 
            int32 len;        
         
            /**
             * Max available number of characters for this string.
             */                 
            int32 max; 
             
        };

        /**
         * A contex of this class containing string.
         */
        Context context_; 

    };
}
#endif // LIBRARY_ABSTRACT_STRING_HPP_
