/** 
 * Abstract string class.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_STRING_HPP_
#define LIBRARY_STRING_HPP_

#include "library.Object.hpp"
#include "api.String.hpp"

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

    public:
    
        /** 
         * Constructor.
         */    
        AbstractString() : Parent(),
            string_        (){
            bool isConstructed = construct("");
            this->setConstruct( isConstructed );
        }
        
        /** 
         * Constructor.
         *
         * @param obj a source string object.
         */ 
        AbstractString(const ::library::String<Type,Alloc>& obj) : Parent(obj),
            string_        (){
            bool isConstructed = construct(obj);                       
            this->setConstruct( isConstructed );
        }
        
        /** 
         * Constructor.
         *
         * @param obj a source string object interface.
         */ 
        String(const ::api::String<Type>& obj) : Parent(obj),
            string_        (){
            bool isConstructed = construct(obj);
            this->setConstruct( isConstructed );
        }
        
        /** 
         * Constructor.
         *
         * @param data a sequence of string characters.
         */    
        String(const Type* data) : Parent(),
            string_        (){
            bool isConstructed = construct(data);            
            this->setConstruct( isConstructed );
        }
    
        /**
         * Destructor.
         */
        virtual ~String()
        {
        }
        
        /** 
         * Assignment operator.
         *
         * @param obj a string object.
         * @return reference to this object.       
         */     
        String& operator=(const ::library::String<Type,Alloc>& obj)
        {
            if( not this->isConstructed_ ) 
            {
                return *this;        
            }
            Parent::operator=(*this);
            bool isConstructed = assign(obj);            
            this->setConstruct( isConstructed );
            return *this;            
        }
        
        /** 
         * Assignment operator.
         *
         * @param obj a string object interface.
         * @return reference to this object.       
         */ 
        String& operator=(const ::api::String<Type>& obj)
        {
            if( not this->isConstructed_ ) 
            {
                return *this;        
            }
            Parent::operator=(*this);
            bool isConstructed = assign(obj);                        
            this->setConstruct( isConstructed );
            return *this;            
        }        
        
        /** 
         * Assignment operator.
         *
         * @param data a sequence of string characters.
         * @return reference to this object.       
         */     
        String& operator=(const Type* data)
        {
            if( not this->isConstructed_ ) 
            {
                return *this;        
            }
            Parent::operator=(*this);
            bool isConstructed = assign(data);             
            this->setConstruct( isConstructed );
            return *this;        
        }
        
        /** 
         * Assignment operator.
         *
         * @param obj a string object.
         * @return reference to this object.       
         */     
        String& operator+=(const ::library::String<Type,Alloc>& obj)
        {
            if( not this->isConstructed_ ) 
            {
                return *this;        
            }
            Parent::operator=(*this);
            bool isConstructed = concatenate(obj);            
            this->setConstruct( isConstructed );
            return *this; 
        }  
        
        /** 
         * Assignment operator.
         *
         * @param obj a string object interface.
         * @return reference to this object.       
         */
        String& operator+=(const ::api::String<Type>& obj)
        {
            if( not this->isConstructed_ ) 
            {
                return *this;        
            }
            Parent::operator=(*this);
            bool isConstructed = concatenate(obj);              
            this->setConstruct( isConstructed );
            return *this; 
        }                 
        
        /** 
         * Assignment operator.
         *
         * @param data a sequence of string characters.
         * @return reference to this object.       
         */     
        String& operator+=(const Type* data)
        {
            if( not this->isConstructed_ ) 
            {
                return *this;        
            }
            Parent::operator=(*this);
            bool isConstructed = concatenate(data);              
            this->setConstruct( isConstructed );
            return *this; 
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
         * Compares two strings lexicographically.
         *
         * @param obj a string object.
         * @return the value 0 if given string is equal to this string; 
         *         a value less than 0 if this string is less than given string; 
         *         a value greater than 0 if this string is greater than given string.
         */
        virtual int32 compareTo(const ::api::String<Type>& obj) const
        {
            if( not obj.isConstructed() ) 
            {
                return 1;
            }
            return compareTo( obj.getChar() );
        }
        
        /** 
         * Compares two strings lexicographically.
         *
         * @param data a sequence of string characters.
         * @return the value 0 if given string is equal to this string; 
         *         a value less than 0 if this string is less than given string; 
         *         a value greater than 0 if this string is greater than given string.
         */
        virtual int32 compareTo(const Type* data) const
        {
            if( not this->isConstructed_ ) 
            {
                return -1;        
            }
            if( data == NULL ) 
            {
                return 1;
            }
            return string_.compareTo( data );        
        }
        
        /**
         * Returns pointer to the first char of containing string.
         *
         * @param first char of containing string or NULL if no string contained.
         */
        virtual const Type* getChar() const
        {
            return this->isConstructed_ ? string_.getData() : NULL;
        }        
        
        /**
         * Returns a number of elements in this container.
         *
         * @return number of elements.
         */
        virtual int32 getLength() const
        {
            return this->isConstructed_ ? string_.getLength() : 0;
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
        
    protected:
    
        /**
         * Container of information about a string.
         */
        template <typename T, class A = Alloc>    
        class StringData : public ::library::Object<A>
        {
        
        public:
        
            /** 
             * Constructor.
             */        
            StringData() :
                data_ (NULL),
                len_ (0),
                max_ (0){
            }

            /**
             * Destructor.
             */
           ~StringData()
            {
                deleteData(data_);
            }  
            
            /** 
             * Copies a string self string.
             *
             * @param data a sequence of string characters.
             * @return true if the task has been completed successfully.             
             */
            bool copy(const T* data)
            {
                if(data == NULL) 
                {
                    return false;            
                }
                int32 len, max;
                len = getLength(data);
                if( not isFit(len) ) 
                {
                    deleteData(data_);
                    max_ = 0;
                }
                if(data_ == NULL)
                {
                    data_ = createData(len, max);
                    max_ = max;
                }
                if(data_ == NULL) 
                {
                    return false;                
                }
                len_ = len;
                copy(data_, data);
                return true;
            } 
            
            /** 
             * Concatenates two strings.
             *
             * @param data a sequence of string characters.             
             * @return true if the task has been completed successfully.             
             */
            bool concatenate(const T* data)
            {
                if(data == NULL) 
                {
                    return false;            
                }
                if(data_ == NULL) 
                {
                    return false;
                }
                int32 len, max;
                len = getLength(data) + len_;          
                if( not isFit(len) ) 
                {
                    T* tmp = createData(len, max);
                    if(tmp != NULL)
                    {
                        copy(tmp, data_);
                    }
                    deleteData(data_);
                    data_ = tmp;
                    max_ = max;                    
                }
                if(data_ == NULL) 
                {
                    return false;                
                }
                len_ = len;
                concatenate(data_, data);
                return true;
            }
            
            /** 
             * Compares two strings lexicographically.
             *
             * @param data a sequence of string characters.
             * @return the value 0 if given string is equal to this string; 
             *         a value less than 0 if this string is shorter than given string; 
             *         a value greater than 0 if this string is longer than given string.
             */
            int32 compareTo(const T* data) const
            {
                int32 val[2];
                int32 res = len_ - getLength(data);
                if(res != 0) 
                {
                    return res;
                }
                for(int32 i=0; i<len_; i++)
                {
                    val[0] = data_[i];
                    val[1] = data[i];
                    res = val[0] - val[1];
                    if(res != 0) 
                    {
                        break;
                    }
                }
                return res;
            }
            
            /**
             * Get pointer to data buffer.
             *
             * @return pointer to data buffer, or NULL.
             */
            const T* getData() const
            {
                return data_;
            }
            
            /**
             * Gets length of string.
             *
             * @return length of string.
             */
            int32 getLength() const
            {
                return len_;
            }                                              
            
        private:
        
            /** 
             * Tests if given length fits to current data buffer.
             *
             * @param len string characters number.
             * @return true if the length will be fit successfully.
             */        
            bool isFit(int32 len)
            {
                return len > max_ ? false : true;
            }        
        
            /** 
             * Creates a new data buffer for string.
             *
             * @param len string characters number.
             * @return the data buffer address.
             */        
            static T* createData(int32 len, int32& max)
            {
                int32 size = calculateSize(len);
                T* data = reinterpret_cast<T*>( A::allocate(size) );
                max = data != NULL ? calculateLength(size) : 0;
                return data;
            }        
            
            /** 
             * Deletes the buffer.
             */        
            static void deleteData(T*& data)
            {
                if(data != NULL) 
                {
                    A::free(data);
                }
                data = NULL;
            }            
            
            /** 
             * Returns size in byte of a string length.
             *
             * @param length string characters number.
             * @return size in byte for given string.
             */
            static int32 calculateSize(int32 len)
            {
                size_t size = static_cast<size_t>(len) * sizeof(T) + sizeof(T);
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
             * @return characters number.
             */
            static int32 calculateLength(int32 size)
            {
                int32 charSize =  static_cast<int32>( sizeof(T) );
                if(charSize == 0) 
                {
                    return 0;
                }
                int32 len = size / charSize;
                return len > 1 ? len - 1 : 0;
            }
            
            /** 
             * Returns a characters number of passed string.
             *
             * @param data a sequence of string characters.
             * @return characters number.
             */
            static int32 getLength(const T* data)
            {
                if(data == NULL) 
                {
                    return 0;
                }
                int32 len = 0;
                while( *data != 0 ) 
                {
                    len++;
                    data++;
                }
                return len;
            }
            
            /** 
             * Copys string to string.
             */
            static void copy(T* dst, const T* src)
            {
              if(dst == NULL || src == NULL) 
              {
                  return;
              }
              T* d = dst - 1;     
              const T* s = src  - 1;     
              while(*++d = *++s);
            }

            /** 
             * Concatenates two strings.
             */
            static void concatenate(T* ptr1, const T* ptr2)
            {
              if(ptr1 == NULL || ptr2 == NULL) 
              {
                  return;
              }
              T* p1 = ptr1 - 1;
              const T* p2 = ptr2 - 1;
              while(*++p1);
              p1--;
              while(*++p1 = *++p2);
            }                                             
        
            /**
             * The first char of containing string.
             */        
            T* data_;
            
            /**
             * Current number of characters of this string.
             */                 
            int32 len_;        

            /**
             * Max number of characters for this string.
             */                 
            int32 max_;
            
        };
        
    private:
    
        /** 
         * Constructs this object.
         *
         * @param obj a source string object.
         * @return true if object has been constructed successfully.         
         */ 
        bool construct(const ::library::String<Type,Alloc>& obj)
        {
            return assign(obj);
        }    
        
        /** 
         * Constructs this object.
         *
         * @param obj a source string object interface.
         * @return true if object has been constructed successfully.         
         */ 
        bool construct(const ::api::String<Type>& obj)
        {
            return assign(obj);
        }        
    
        /**
         * Constructs this object.
         *
         * @param data a sequence of string characters.
         * @return true if object has been constructed successfully.
         */
        bool construct(const Type* data)
        {
            return assign(data);
        }

        /** 
         * Assigns a new string.
         *
         * @param obj a source string object.
         * @return true if the task has been completed successfully.
         */
        bool assign(const ::library::String<Type,Alloc>& obj)
        {
            const ::api::String<Type>& iobj = obj;        
            return assign( iobj );
        }
        
        /** 
         * Assigns a new string.
         *
         * @param obj a source string object interface.
         * @return true if the task has been completed successfully.
         */
        bool assign(const ::api::String<Type>& obj)
        {
            if( not obj.isConstructed() ) 
            {
                return false;
            }
            return assign( obj.getChar() );
        }        
        
        /** 
         * Assigns a new string.
         *
         * @param data a sequence of string characters.
         * @return true if the task has been completed successfully.
         */
        bool assign(const Type* data)
        {
            if( not this->isConstructed_ ) 
            {
                return false;        
            }
            return string_.copy(data);
        }
        
        /** 
         * Concatenates self string and new string.
         *
         * @param obj a source string object.
         * @return true if the task has been completed successfully.
         */
        bool concatenate(const ::library::String<Type,Alloc>& obj)
        {
            const ::api::String<Type>& iobj = obj;
            return concatenate( iobj );
        }
        
        /** 
         * Concatenates self string and new string.
         *
         * @param obj a source string object interface.
         * @return true if the task has been completed successfully.
         */
        bool concatenate(const ::api::String<Type>& obj)
        {
            if( not obj.isConstructed() ) 
            {
                return false;
            }
            return concatenate( obj.getChar() );
        }                        
        
        /** 
         * Concatenates self string and new string.
         *
         * @param str reference to String object.
         * @return true if the task has been completed successfully.
         */
        bool concatenate(const Type* data)
        {
            if( not this->isConstructed_ ) 
            {
                return false;        
            }
            return string_.concatenate(data);
        }

        /**
         * Container of information about a string.
         */
        StringData string_;

    };
    
    /**
     * Concatenates two strings.
     *
     * @param obj1 a string 1 object.
     * @param obj2 a string 2 object.
     * @return a new string object.
     */
    template <typename Type, class Alloc>
    inline ::library::String<Type,Alloc> operator+(const ::library::String<Type,Alloc>& obj1, const ::library::String<Type,Alloc>& obj2)
    {
        String<Type,Alloc> str = obj1;
        str += obj2;
        return str;
    }
    
    /**
     * Concatenates two strings.
     *
     * @param obj1 a string 1 object.
     * @param obj2 a string 2 object interface.
     * @return a new string object.
     */
    template <typename Type, class Alloc>
    inline ::library::String<Type,Alloc> operator+(const ::library::String<Type,Alloc>& obj1, const ::api::String<Type>& obj2)
    {
        String<Type,Alloc> str = obj1;
        str += obj2;
        return str;
    }    
    
    /**
     * Concatenates two strings.
     *
     * @param obj1 a string 1 object interface.
     * @param obj2 a string 2 object.
     * @return a new string object.
     */
    template <typename Type, class Alloc>
    inline ::library::String<Type,Alloc> operator+(const ::api::String<Type>& obj1, const ::library::String<Type,Alloc>& obj2)
    {
        String<Type,Alloc> str = obj1;
        str += obj2;
        return str;
    }     
    
    /**
     * Concatenates two strings.
     *
     * @param obj a string object.
     * @param data a sequence of string characters.
     * @return a new string object.
     */         
    template <typename Type, class Alloc>
    inline ::library::String<Type,Alloc> operator+(const ::library::String<Type,Alloc>& obj, const Type* data)
    {
        String<Type,Alloc> str = obj;
        str += data;
        return str;
    } 
    
    /**
     * Concatenates two strings.
     *
     * @param data a sequence of string characters.     
     * @param obj a string object.
     * @return a new string object.
     */
    template <typename Type, class Alloc>
    inline ::library::String<Type,Alloc> operator+(const Type* data, const ::library::String<Type,Alloc>& obj)
    {
        String<Type,Alloc> str = data;
        str += obj;
        return str;
    }   

    /**
     * Compares for equality of two strings.
     *
     * @param obj1 a string 1 object.
     * @param obj2 a string 2 object.
     * @return true if strings are equal.
     */
    template <typename Type, class Alloc>
    inline bool operator==(const ::library::String<Type,Alloc>& obj1, const ::library::String<Type,Alloc>& obj2)
    {
        return obj1.compareTo(obj2) == 0 ? true : false;
    }
    
    /**
     * Compares for equality of two strings.
     *
     * @param obj1 a string 1 object.
     * @param obj2 a string 2 object interface.
     * @return true if strings are equal.
     */
    template <typename Type, class Alloc>
    inline bool operator==(const ::library::String<Type,Alloc>& obj1, const ::api::String<Type>& obj2)
    {
        return obj1.compareTo(obj2) == 0 ? true : false;
    } 
    
    /**
     * Compares for equality of two strings.
     *
     * @param obj1 a string 1 object interface.
     * @param obj2 a string 2 object.
     * @return true if strings are equal.
     */
    template <typename Type, class Alloc>
    inline bool operator==(const ::api::String<Type>& obj1, const ::library::String<Type,Alloc>& obj2)
    {
        return obj1.compareTo(obj2) == 0 ? true : false;
    }       
    
    /**
     * Compares for equality of two strings.
     *
     * @param obj a string object.
     * @param data a sequence of string characters.
     * @return true if strings are equal.
     */
    template <typename Type, class Alloc>
    inline bool operator==(const ::library::String<Type,Alloc>& obj, const Type* data)
    {
        return obj.compareTo(data) == 0 ? true : false;
    }
    
    /**
     * Compares for equality of two strings.
     *
     * @param data a sequence of string characters.     
     * @param obj a string object.
     * @return true if strings are equal.
     */
    template <typename Type, class Alloc>
    inline bool operator==(const Type* data, const ::library::String<Type,Alloc>& obj)
    {
        return obj.compareTo(data) == 0 ? true : false;
    }    
    
    /**
     * Compares for inequality of two strings.
     *
     * @param obj1 a string 1 object.
     * @param obj2 a string 2 object.
     * @return true if strings are not equal.
     */      
    template <typename Type, class Alloc>
    inline bool operator!=(const ::library::String<Type,Alloc>& obj1, const ::library::String<Type,Alloc>& obj2)     
    {
        return obj1.compareTo(obj2) == 0 ? false : true;
    } 
    
    /**
     * Compares for inequality of two strings.
     *
     * @param obj1 a string 1 object.
     * @param obj2 a string 2 object interface.
     * @return true if strings are not equal.
     */      
    template <typename Type, class Alloc>
    inline bool operator!=(const ::library::String<Type,Alloc>& obj1, const ::api::String<Type>& obj2)     
    {
        return obj1.compareTo(obj2) == 0 ? false : true;
    }
    
    /**
     * Compares for inequality of two strings.
     *
     * @param obj1 a string 1 object interface.
     * @param obj2 a string 2 object.
     * @return true if strings are not equal.
     */      
    template <typename Type, class Alloc>
    inline bool operator!=(const ::api::String<Type>& obj1, const ::library::String<Type,Alloc>& obj2)     
    {
        return obj1.compareTo(obj2) == 0 ? false : true;
    }    
    
    /**
     * Compares for inequality of two strings.
     *
     * @param obj a string object.
     * @param data a sequence of string characters.
     * @return true if strings are not equal.
     */
    template <typename Type, class Alloc>
    inline bool operator!=(const ::library::String<Type,Alloc>& obj, const Type* data)
    {
        return obj.compareTo(data) == 0 ? false : true;
    }
    
    /**
     * Compares for inequality of two strings.
     *
     * @param data a sequence of string characters.     
     * @param obj a string object.
     * @return true if strings are not equal.
     */
    template <typename Type, class Alloc>
    inline bool operator!=(const Type* data, const ::library::String<Type,Alloc>& obj)
    {
        return obj.compareTo(data) == 0 ? false : true;
    }

}
#endif // LIBRARY_STRING_HPP_
