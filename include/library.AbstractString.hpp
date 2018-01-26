/** 
 * Abstract string class.
 *
 * This class has a primary template and a partial specialization of the template. 
 * The non-specialized template defines a realization that contains a whole buffer for 
 * string characters, which is defined by a template argument, as data member of the class.
 * The specialization allocates necessary memory size for containing the buffer 
 * in a heap memory. 
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017-2018, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_ABSTRACT_STRING_HPP_
#define LIBRARY_ABSTRACT_STRING_HPP_

#include "library.AbstractBaseString.hpp"

namespace library
{
    /**
     * Primary template implements the static string class.
     *
     * @param Type       data type of string characters.     
     * @param MAX_LENGTH maximum number of string characters, or 0 for dynamic allocation.
     * @param Alloc      heap memory allocator class.
     */
    template <typename Type, int32 MAX_LENGTH, class Alloc = Allocator>    
    class AbstractString : public ::library::AbstractBaseString<Type,Alloc>
    {
        typedef ::library::AbstractBaseString<Type,Alloc> Parent;
        
        using Parent::copy;
        using Parent::concatenate;
        using Parent::compare;         

    public:
    
        /** 
         * Constructor.
         */    
        AbstractString() : Parent()
        {
        }     
        
        /**
         * Destructor.
         */
        virtual ~AbstractString()
        {
        } 
        
        /**
         * Returns a number of elements in this container.
         *
         * @return number of elements.
         */
        virtual int32 getLength() const
        {
            return context_.len;
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
         * Copies a passed string into this string.
         *
         * @param str a character string to be copied.
         * @return true if a passed string has been copied successfully.
         */
        virtual bool copy(const Type* str)
        {
            if( not this->isConstructed_ || str == NULL )
            {
                return false;
            }        
            int32 len = Parent::getLength(str);
            // If a given string length is more than this max available length
            if( not context_.isFit(len) ) 
            {
                // Create a new temporary string context
                Context context;
                if( not context.allocate(len) )
                {
                    return false;                
                }
                // Delete this string context
                context_.free();                
                // Set new contex                
                context_.mirror(context);
            }
            else
            {
                context_.len = len;
            }
            Parent::copy(context_.str, str);                                    
            return true;          
        }
        
        /** 
         * Concatenates a passed string to this string.             
         *
         * @param str an character string to be appended.             
         * @return true if a passed string has been appended successfully.          
         */
        virtual bool concatenate(const Type* str)
        {
            if( not this->isConstructed_ || str == NULL )
            {
                return false;
            }                
            // Simply, would copy a given string if the context were freed
            if( not context_.isAllocated() )
            {
                return copy(str);
            }
            int32 len = Parent::getLength(str) + context_.len;          
            // If a length of this string plus a given string is more than this max available length            
            if( not context_.isFit(len) ) 
            {
                // Create a new temporary string context
                Context context;
                if( not context.allocate(len) )
                {
                    return false;                
                }
                // Copy a string of this context to the new contex string
                Parent::copy(context.str, context_.str);
                // Delete this string context
                context_.free();
                // Set new contex
                context_.mirror(context);                   
            }
            else
            {
                context_.len = len;
            }
            Parent::concatenate(context_.str, str);
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
        virtual int32 compare(const Type* str) const
        {
            if( not this->isConstructed_ || context_.str == NULL || str == NULL )
            {
                return Parent::MINIMUM_POSSIBLE_VALUE_OF_INT32;
            }                
            int32 val[2];
            int32 res = context_.len - Parent::getLength(str);
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
         * Constructor.
         *
         * NOTE: Creating this object based on another object is prohibited. 
         * The operation is performed by calling the default object constructor 
         * and the copy interface function after that. This sequence of calls is 
         * determined because the copy function does not deconstruct the object.
         *
         * @param obj a source object.
         */
        AbstractString(const AbstractString<Type,MAX_LENGTH,Alloc>& obj);
        
        /**
         * Direct assignment operator.
         *
         * The assignment to this object any values of another object is prohibited. 
         * The operation is performed by calling the copy interface function, 
         * because it does not deconstruct the object.
         *
         * @param obj a source object.
         * @return this object.     
         */
        AbstractString<Type,MAX_LENGTH,Alloc>& operator =(const AbstractString<Type,MAX_LENGTH,Alloc>& obj);    
    
        /**
         * A contex of this class containing string.
         */        
        struct Context
        {
        
        public:
        
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
             * Mirrors an context to this.
             *
             * @param obj a source object.
             */
            void mirror(const Context& obj)
            {
                // Don't instigate a copy of object buffer to this buffer, 
                // as the source object always is temporary and 
                // doesn't have any information in its buffer. 
                str = obj.str != NULL ? buf_ : NULL;
                len = obj.len;        
                max = obj.max;              
            }            
            
            /** 
             * Allocates this contex.
             *
             * @param length a number of string characters.
             * @return true if the context has been allocated successfully.
             */
            bool allocate(int32 length)
            {
                if(str != NULL)
                {
                    return false;
                }
                if(length > MAX_LENGTH)
                {
                    return false;
                }
                // Set this class variables
                str = buf_;
                len = length;                
                max = MAX_LENGTH;
                return true;
            }
            
            /** 
             * Frees this contex.
             */
            void free()
            {
                str = NULL;
                len = 0;            
                max = 0;            
            }
            
            /** 
             * Test if this contex is allocated.
             *
             * @return true if the context has been allocated successfully.
             */            
            bool isAllocated()
            {
                return str == NULL ? false : true;
            }
            
            /** 
             * Tests if a passed length fits to this max available length.
             *
             * @param len a number of string characters.
             * @return true if this length will be fit successfully.
             */        
            bool isFit(int32 len) const
            {
                return len > max ? false : true;
            }            
        
        private:
        
            /** 
             * Constructor.
             *
             * @param obj a source object.
             */
            Context(const Context& context);
            
            /**
             * Assignment operator.
             *
             * @param obj a source object.
             * @return this object.     
             */
            Context& operator =(const Context& obj);            
            
            /**
             * The buffer of characters of this string.
             */        
            Type buf_[MAX_LENGTH + 1];                  
             
        };

        /**
         * A contex of this class containing string.
         */
        Context context_;

    };
    
    #ifdef NO_STRICT_MISRA_RULES
    
    /** 
     * Partial specialization of the template implements the dynamic string class.
     *
     * @param Type  data type of string characters.     
     * @param Alloc heap memory allocator class.
     */
    template <typename Type, class Alloc>
    class AbstractString<Type,0,Alloc> : public AbstractBaseString<Type,Alloc>    
    {
        typedef ::library::AbstractBaseString<Type,Alloc> Parent;

        using Parent::copy;
        using Parent::concatenate;
        using Parent::compare;        

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
            context_.free();        
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
         * Copies a passed string into this string.
         *
         * @param str a character string to be copied.
         * @return true if a passed string has been copied successfully.
         */
        virtual bool copy(const Type* str)
        {
            if( not this->isConstructed_ || str == NULL )
            {
                return false;
            }        
            int32 len = Parent::getLength(str);
            // If a given string length is more than this max available length
            if( not context_.isFit(len) ) 
            {
                // Create a new temporary string context
                Context context;
                if( not context.allocate(len) )
                {
                    return false;                
                }
                // Delete this string context
                context_.free();                
                // Set new contex                
                context_.mirror(context);
            }
            else
            {
                context_.len = len;
            }
            Parent::copy(context_.str, str);                                    
            return true;        
        }
        
        /** 
         * Concatenates a passed string to this string.             
         *
         * @param str an character string to be appended.             
         * @return true if a passed string has been appended successfully.          
         */
        virtual bool concatenate(const Type* str)
        {
            if( not this->isConstructed_ || str == NULL )
            {
                return false;
            }                
            // Simply, would copy a given string if the context were freed
            if( not context_.isAllocated() )
            {
                return copy(str);
            }
            int32 len = Parent::getLength(str) + context_.len;          
            // If a length of this string plus a given string is more than this max available length            
            if( not context_.isFit(len) ) 
            {
                // Create a new temporary string context
                Context context;
                if( not context.allocate(len) )
                {
                    return false;                
                }
                // Copy a string of this context to the new contex string
                Parent::copy(context.str, context_.str);
                // Delete this string context
                context_.free();
                // Set new contex
                context_.mirror(context);
            }
            else
            {
                context_.len = len;
            }
            Parent::concatenate(context_.str, str);
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
        virtual int32 compare(const Type* str) const
        {
            if( not this->isConstructed_ || context_.str == NULL || str == NULL )
            {
                return Parent::MINIMUM_POSSIBLE_VALUE_OF_INT32;
            }                
            int32 val[2];
            int32 res = context_.len - Parent::getLength(str);
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
         * Constructor.
         *
         * NOTE: Creating this object based on another object is prohibited. 
         * The operation is performed by calling the default object constructor 
         * and the copy interface function after that. This sequence of calls is 
         * determined because the copy function does not deconstruct the object.
         *
         * @param obj a source object.
         */
        AbstractString(const AbstractString<Type,0,Alloc>& obj);
        
        /**
         * Direct assignment operator.
         *
         * The assignment to this object any values of another object is prohibited. 
         * The operation is performed by calling the copy interface function, 
         * because it does not deconstruct the object.
         *
         * @param obj a source object.
         * @return this object.     
         */
        AbstractString<Type,0,Alloc>& operator =(const AbstractString<Type,0,Alloc>& obj);
                
        /**
         * A contex of this class containing string.
         */        
        struct Context
        {
        
        public:
        
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
             * Mirrors an context to this.
             *
             * @param obj a source object.
             */
            void mirror(const Context& obj)
            {
                // Copy an argument pointer value to this pointer value, 
                // as we know that the source object will be destroyed 
                // immediately after this function called without deleting 
                // of the object allocated heap for the array of its characters.
                str = obj.str;
                len = obj.len;        
                max = obj.max;              
            }             
            
            /** 
             * Allocates this contex.
             *
             * @param length a number of string characters.
             * @return true if the context has been allocated successfully.
             */
            bool allocate(int32 length)
            {
                if(str != NULL)
                {
                    return false;
                }
                // Calculate size in byte for the given length
                int32 size = calculateSize(length);
                // Allocate a new array
                Type* string = reinterpret_cast<Type*>( Alloc::allocate(size) );
                if(string == NULL)
                {
                    return false;                
                }          
                // Set this class variables
                str = string;
                len = length;                
                max = calculateLength(size);
                return true;
            }
            
            /** 
             * Frees this contex.
             */
            void free()
            {
                if(str != NULL) 
                {
                    Alloc::free(str);                
                    str = NULL;
                    len = 0;            
                    max = 0;            
                }
            }
            
            /** 
             * Test if this contex is allocated.
             *
             * @return true if the context has been allocated successfully.
             */            
            bool isAllocated()
            {
                return str == NULL ? false : true;
            }
            
            /** 
             * Tests if a passed length fits to this max available length.
             *
             * @param len a number of string characters.
             * @return true if this length will be fit successfully.
             */        
            bool isFit(int32 len) const
            {
                return len > max ? false : true;
            } 
        
        private:
            
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
                int32 charSize = static_cast<int32>( sizeof(Type) );
                if(charSize == 0) 
                {
                    return 0;
                }
                int32 len = size / charSize;
                return len > 1 ? len - 1 : 0;
            }
            
            /** 
             * Constructor.
             *
             * @param obj a source object.
             */
            Context(const Context& context);
            
            /**
             * Assignment operator.
             *
             * @param obj a source object.
             * @return this object.     
             */
            Context& operator =(const Context& obj);             
            
        };

        /**
         * A contex of this class containing string.
         */
        Context context_;

    };

    #endif // NO_STRICT_MISRA_RULES

}
#endif // LIBRARY_ABSTRACT_STRING_HPP_
