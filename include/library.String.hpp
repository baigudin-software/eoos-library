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
#include "library.Memory.hpp"

namespace global
{
    namespace library
    {
        /**
         * Primary template implements the static string class.
         *
         * @param T data type of string characters.     
         * @param L maximum number of string characters, or 0 for dynamic allocation.
         * @param A heap memory allocator class.
         */
        template <typename T, int32 L, class A = Allocator>    
        class String : public library::AbstractString<T,L,A>
        {
            typedef library::AbstractString<T,L,A> Parent;
            
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
            String(const T* source) : Parent()
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
            virtual T getTerminator() const
            {
                return T::TERMINATING_CHARACTER;
            }
    
        };
            
        /** 
         * Char type partial specialization of the static string class.
         *
         * @param A heap memory allocator class.
         */
        template <int32 L, class A>        
        class String<char,L,A> : public library::AbstractString<char,L,A>
        {
            typedef library::AbstractString<char,L,A> Parent;
    
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
            String(const library::String<char,L,A>& source) : Parent()       
            {
                this->copy(source);        
            }    
            
            /** 
             * Constructor.
             *
             * @param source a source object interface.
             */             
            String(const api::String<char>& source) : Parent()       
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
             * Constructor.
             *
             * @param value a source numerical value.
             * @param base a numerical base used to represent a value as a string.         
             */    
            String(const int32 value) : Parent()
            {     
                this->convert<int32>(value, 10);
            }        
            
            /**
             * Casts to int32 type.
             *
             * @return a numerical value.     
             */        
            operator int32() const 
            { 
                return this->template cast<int32>(10);
            }        
            
            /**
             * Direct assignment operator.
             *
             * @param source a source object.
             * @return this object.     
             */
            library::String<char,L,A>& operator =(const library::String<char,L,A>& source)
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
            library::String<char,L,A>& operator =(const api::String<char>& source)
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
            library::String<char,L,A>& operator =(const char* source)
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
            library::String<char,L,A>& operator +=(const library::String<char,L,A>& source)
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
            library::String<char,L,A>& operator +=(const api::String<char>& source)
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
            library::String<char,L,A>& operator +=(const char* source)
            {
                this->concatenate(source);
                return *this;
            }
            
            /** 
             * Assignment by sum operator.
             *
             * @param value a source numerical value.
             * @return reference to this object.       
             */     
            library::String<char,L,A>& operator +=(const int32 value)
            {
                const String string = value;
                this->concatenate(string);
                return *this;
            }        
            
            /** 
             * Converts an integer number to this string.
             *
             * The function converts an integer value into a character string using the base parameter, 
             * which has to be 2, 8, 10, or 16 based numerals for converting to an appropriate numeral system.
             * 
             * Mark that only if the base is decimal, a passed number is available to be negative values, 
             * and the resulting string of these values is preceded with a minus sign. In addition, 
             * a hexadecimal number includes lower case characters, and any resulting strings do not contain 
             * any suffixes or prefixes for identifying a numeral system.
             *    
             * NOTE: You need to use "string.template convert<I>(value, base);" syntax, 
             * if you have to specify the template argument type explicitly.
             *
             * @param val  a value that would be converted to this string.
             * @param base a numerical base used to represent a value as this string.
             * @return true if the conversion has been completed successfully.
             */        
            template <typename I>
            bool convert(const I value, const int32 base = 10)
            {
                static const int32 MAX_LENGTH = sizeof(I) * 8 + 1;           
                char temp[MAX_LENGTH]; 
                if( not Memory::itoa<I>(value, temp, base) )                    
                {
                    return false;
                }
                return this->copy(temp);
            }  
            
            /** 
             * Casts this string to an integer number.
             *
             * NOTE: You need to use "string.template cast<I>(base);" syntax,
             * if you have to specify the template argument type explicitly.         
             *
             * @param base a numerical base used to parse the string.
             * @return the resulting number.
             */        
            template <typename I>
            I cast(const int32 base = 10) const
            {
                return Memory::atoi<I>(this->getChar(), base); 
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
            
        private:
            
            template <int32 L0, class A0> friend bool operator ==(const library::String<char,L0,A0>&, const char*);
            template <int32 L0, class A0> friend bool operator ==(const char*, const library::String<char,L0,A0>&);
            template <int32 L0, class A0> friend bool operator !=(const library::String<char,L0,A0>&, const char*);
            template <int32 L0, class A0> friend bool operator !=(const char*, const library::String<char,L0,A0>&);   
        };
        
        /**
         * Compares for equality of two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source object 2.
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline bool operator ==(const library::String<char,L,A>& source1, const library::String<char,L,A>& source2)
        {
            return source1.compare(source2) == 0 ? true : false;
        }
        
        /**
         * Compares for equality of two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source object interface 2.
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline bool operator ==(const library::String<char,L,A>& source1, const api::String<char>& source2)
        {
            return source1.compare(source2) == 0 ? true : false;
        } 
        
        /**
         * Compares for equality of two strings.
         *
         * @param source1 a source object interface 1.
         * @param source2 a source object 2.
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline bool operator ==(const api::String<char>& source1, const library::String<char,L,A>& source2)
        {
            return source1.compare(source2) == 0 ? true : false;
        }
    
        /**
         * Compares for equality of two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source character string 2.
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline bool operator ==(const library::String<char,L,A>& source1, const char* source2)
        {
            return source1.compare(source2) == 0 ? true : false;
        }
    
        /**
         * Compares for equality of two strings.
         *
         * @param source1 a source character string 1.
         * @param source2 a source source object 2. 
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline bool operator ==(const char* source1, const library::String<char,L,A>& source2)
        {
            return source2.compare(source1) == 0 ? true : false;
        } 
        
        /**
         * Compares for inequality of two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source object 2.
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline bool operator !=(const library::String<char,L,A>& source1, const library::String<char,L,A>& source2)
        {
            return source1.compare(source2) != 0 ? true : false;
        }
        
        /**
         * Compares for inequality of two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source object interface 2.
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline bool operator !=(const library::String<char,L,A>& source1, const api::String<char>& source2)
        {
            return source1.compare(source2) != 0 ? true : false;
        } 
        
        /**
         * Compares for inequality of two strings.
         *
         * @param source1 a source object interface 1.
         * @param source2 a source object 2.
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline bool operator !=(const api::String<char>& source1, const library::String<char,L,A>& source2)
        {
            return source1.compare(source2) != 0 ? true : false;
        }
    
        /**
         * Compares for inequality of two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source character string 2.
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline bool operator !=(const library::String<char,L,A>& source1, const char* source2)
        {
            return source1.compare(source2) != 0 ? true : false;
        }
    
        /**
         * Compares for inequality of two strings.
         *
         * @param source1 a source character string 1.
         * @param source2 a source source object 2. 
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline bool operator !=(const char* source1, const library::String<char,L,A>& source2)
        {
            return source2.compare(source1) != 0 ? true : false;
        }  
        
        /**
         * Concatenates two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source object 2.
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline library::String<char,L,A> operator +(const library::String<char,L,A>& source1, const library::String<char,L,A>& source2)
        {
            library::String<char,L,A> string( source1 );
            string += source2;
            return string;    
        }
        
        /**
         * Concatenates two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source object interface 2.
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline library::String<char,L,A> operator +(const library::String<char,L,A>& source1, const api::String<char>& source2)
        {
            library::String<char,L,A> string( source1 );
            string += source2;
            return string;    
        } 
        
        /**
         * Concatenates two strings.
         *
         * @param source1 a source object interface 1.
         * @param source2 a source object 2.
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline library::String<char,L,A> operator +(const api::String<char>& source1, const library::String<char,L,A>& source2)
        {
            library::String<char,L,A> string( source1 );
            string += source2;
            return string;    
        }
    
        /**
         * Concatenates two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source character string 2.
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline library::String<char,L,A> operator +(const library::String<char,L,A>& source1, const char* source2)
        {
            library::String<char,L,A> string( source1 );
            string += source2;
            return string;    
        }
    
        /**
         * Concatenates two strings.
         *
         * @param source1 a source character string 1.
         * @param source2 a source source object 2. 
         * @return true if strings are equal.
         */
        template <int32 L, class A>
        inline library::String<char,L,A> operator +(const char* source1, const library::String<char,L,A>& source2)
        {
            library::String<char,L,A> string( source1 );
            string += source2;
            return string;    
        }    
        
        #ifdef NO_STRICT_MISRA_RULES  
        
        /** 
         * Partial specialization of the template implements the dynamic string class.
         *
         * @param T data type of string characters.
         * @param A heap memory allocator class.
         */
        template <typename T, class A>    
        class String<T,0,A> : public library::AbstractString<T,0,A>
        {
            typedef library::AbstractString<T,0,A> Parent;
    
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
            String(const T* source) : Parent()
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
            virtual T getTerminator() const
            {
                return T::TERMINATING_CHARACTER;
            }                        
        
        };
    
        /** 
         * Char type partial specialization of the dynamic string class.
         *
         * @param A heap memory allocator class.
         */
        template <class A>        
        class String<char,0,A> : public library::AbstractString<char,0,A>
        {
            typedef library::AbstractString<char,0,A> Parent;
    
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
            String(const library::String<char,0,A>& source) : Parent()       
            {
                this->copy(source);        
            }    
            
            /** 
             * Constructor.
             *
             * @param source a source object interface.
             */             
            String(const api::String<char>& source) : Parent()       
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
             * Constructor.
             *
             * @param value a source numerical value.
             * @param base a numerical base used to represent a value as a string.         
             */    
            String(int32 value) : Parent()
            {     
                this->convert<int32>(value, 10);
            }        
    
            /**
             * Casts to int32 type.
             *
             * @return a numerical value.     
             */        
            operator int32() const 
            { 
                return this->template cast<int32>(10);
            }        
            
            /**
             * Direct assignment operator.
             *
             * @param source a source object.
             * @return this object.     
             */
            library::String<char,0,A>& operator =(const library::String<char,0,A>& source)
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
            library::String<char,0,A>& operator =(const api::String<char>& source)
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
            library::String<char,0,A>& operator =(const char* source)
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
            library::String<char,0,A>& operator +=(const library::String<char,0,A>& source)
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
            library::String<char,0,A>& operator +=(const api::String<char>& source)
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
            library::String<char,0,A>& operator +=(const char* source)
            {
                this->concatenate(source);
                return *this;
            }
            
            /** 
             * Assignment by sum operator.
             *
             * @param value a source numerical value.
             * @return reference to this object.       
             */     
            library::String<char,0,A>& operator +=(int32 value)
            {
                String string = value;
                this->concatenate(string);
                return *this;
            }        
            
            /** 
             * Converts an integer number to this string.
             *
             * The function converts an integer value into a character string using the base parameter, 
             * which has to be 2, 8, 10, or 16 based numerals for converting to an appropriate numeral system.
             * 
             * Mark that only if the base is decimal, a passed number is available to be negative values, 
             * and the resulting string of these values is preceded with a minus sign. In addition, 
             * a hexadecimal number includes lower case characters, and any resulting strings do not contain 
             * any suffixes or prefixes for identifying a numeral system.
             *    
             * NOTE: You need to use "string.template convert<I>(value, base);" syntax, 
             * if you have to specify the template argument type explicitly.
             *
             * @param val  a value that would be converted to this string.
             * @param base a numerical base used to represent a value as this string.
             * @return true if the conversion has been completed successfully.
             */        
            template <typename I>
            bool convert(I value, int32 base = 10)
            {
                static const int32 MAX_LENGTH = sizeof(I) * 8 + 1;           
                char temp[MAX_LENGTH]; 
                if( not Memory::itoa<I>(value, temp, base) )                    
                {
                    return false;
                }
                return this->copy(temp);
            }  
            
            /** 
             * Casts this string to an integer number.
             *
             * NOTE: You need to use "string.template cast<I>(base);" syntax,
             * if you have to specify the template argument type explicitly.         
             *
             * @param base a numerical base used to parse the string.
             * @return the resulting number.
             */        
            template <typename I>
            I cast(int32 base = 10) const
            {
                return Memory::atoi<I>(this->getChar(), base); 
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
            
        private:
            
            template <class A0> friend bool operator ==(const library::String<char,0,A0>&, const char*);
            template <class A0> friend bool operator ==(const char*, const library::String<char,0,A0>&);
            template <class A0> friend bool operator !=(const library::String<char,0,A0>&, const char*);
            template <class A0> friend bool operator !=(const char*, const library::String<char,0,A0>&);        
        };
        
        /**
         * Compares for equality of two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source object 2.
         * @return true if strings are equal.
         */
        template <class A>
        inline bool operator ==(const library::String<char,0,A>& source1, const library::String<char,0,A>& source2)
        {
            return source1.compare(source2) == 0 ? true : false;
        }
        
        /**
         * Compares for equality of two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source object interface 2.
         * @return true if strings are equal.
         */
        template <class A>
        inline bool operator ==(const library::String<char,0,A>& source1, const api::String<char>& source2)
        {
            return source1.compare(source2) == 0 ? true : false;
        } 
        
        /**
         * Compares for equality of two strings.
         *
         * @param source1 a source object interface 1.
         * @param source2 a source object 2.
         * @return true if strings are equal.
         */
        template <class A>
        inline bool operator ==(const api::String<char>& source1, const library::String<char,0,A>& source2)
        {
            return source1.compare(source2) == 0 ? true : false;
        }
    
        /**
         * Compares for equality of two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source character string 2.
         * @return true if strings are equal.
         */
        template <class A>
        inline bool operator ==(const library::String<char,0,A>& source1, const char* source2)
        {
            return source1.compare(source2) == 0 ? true : false;
        }
    
        /**
         * Compares for equality of two strings.
         *
         * @param source1 a source character string 1.
         * @param source2 a source source object 2. 
         * @return true if strings are equal.
         */
        template <class A>
        inline bool operator ==(const char* source1, const library::String<char,0,A>& source2)
        {
            return source2.compare(source1) == 0 ? true : false;
        } 
        
        /**
         * Compares for inequality of two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source object 2.
         * @return true if strings are equal.
         */
        template <class A>
        inline bool operator !=(const library::String<char,0,A>& source1, const library::String<char,0,A>& source2)
        {
            return source1.compare(source2) != 0 ? true : false;
        }
        
        /**
         * Compares for inequality of two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source object interface 2.
         * @return true if strings are equal.
         */
        template <class A>
        inline bool operator !=(const library::String<char,0,A>& source1, const api::String<char>& source2)
        {
            return source1.compare(source2) != 0 ? true : false;
        } 
        
        /**
         * Compares for inequality of two strings.
         *
         * @param source1 a source object interface 1.
         * @param source2 a source object 2.
         * @return true if strings are equal.
         */
        template <class A>
        inline bool operator !=(const api::String<char>& source1, const library::String<char,0,A>& source2)
        {
            return source1.compare(source2) != 0 ? true : false;
        }
    
        /**
         * Compares for inequality of two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source character string 2.
         * @return true if strings are equal.
         */
        template <class A>
        inline bool operator !=(const library::String<char,0,A>& source1, const char* source2)
        {
            return source1.compare(source2) != 0 ? true : false;
        }
    
        /**
         * Compares for inequality of two strings.
         *
         * @param source1 a source character string 1.
         * @param source2 a source source object 2. 
         * @return true if strings are equal.
         */
        template <class A>
        inline bool operator !=(const char* source1, const library::String<char,0,A>& source2)
        {
            return source2.compare(source1) != 0 ? true : false;
        }  
        
        /**
         * Concatenates two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source object 2.
         * @return true if strings are equal.
         */
        template <class A>
        inline library::String<char,0,A> operator +(const library::String<char,0,A>& source1, const library::String<char,0,A>& source2)
        {
            library::String<char,0,A> string( source1 );
            string += source2;
            return string;    
        }
        
        /**
         * Concatenates two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source object interface 2.
         * @return true if strings are equal.
         */
        template <class A>
        inline library::String<char,0,A> operator +(const library::String<char,0,A>& source1, const api::String<char>& source2)
        {
            library::String<char,0,A> string( source1 );
            string += source2;
            return string;    
        } 
        
        /**
         * Concatenates two strings.
         *
         * @param source1 a source object interface 1.
         * @param source2 a source object 2.
         * @return true if strings are equal.
         */
        template <class A>
        inline library::String<char,0,A> operator +(const api::String<char>& source1, const library::String<char,0,A>& source2)
        {
            library::String<char,0,A> string( source1 );
            string += source2;
            return string;    
        }
    
        /**
         * Concatenates two strings.
         *
         * @param source1 a source object 1.
         * @param source2 a source character string 2.
         * @return true if strings are equal.
         */
        template <class A>
        inline library::String<char,0,A> operator +(const library::String<char,0,A>& source1, const char* source2)
        {
            library::String<char,0,A> string( source1 );
            string += source2;
            return string;    
        }
    
        /**
         * Concatenates two strings.
         *
         * @param source1 a source character string 1.
         * @param source2 a source source object 2. 
         * @return true if strings are equal.
         */
        template <class A>
        inline library::String<char,0,A> operator +(const char* source1, const library::String<char,0,A>& source2)
        {
            library::String<char,0,A> string( source1 );
            string += source2;
            return string;    
        }        
    
        #endif // NO_STRICT_MISRA_RULES
        
    }
}
#endif // LIBRARY_STRING_HPP_
