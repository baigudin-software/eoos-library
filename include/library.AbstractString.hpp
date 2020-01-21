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
 * @copyright 2017-2018, Sergey Baigudin, Baigudin Software
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_ABSTRACT_STRING_HPP_
#define LIBRARY_ABSTRACT_STRING_HPP_

#include "library.AbstractBaseString.hpp"

namespace local
{
    namespace library
    {
        /**
         * Primary template implements the static string class.
         *
         * @param T - a data type of string characters.
         * @param L - a maximum number of string characters, or 0 for dynamic allocation.
         * @param A - a heap memory allocator class.
         */
        template <typename T, int32 L, class A = Allocator>
        class AbstractString : public library::AbstractBaseString<T,A>
        {
            typedef library::AbstractString<T,L,A>    Self;
            typedef library::AbstractBaseString<T,A>  Parent;

        public:

            using Parent::copy;
            using Parent::concatenate;
            using Parent::compare;

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
            virtual const T* getChar() const
            {
                return context_.str;
            }

        protected:

            /**
             * Copies a passed string into this string.
             *
             * @param str - a character string to be copied.
             * @return true if a passed string has been copied successfully.
             */
            virtual bool copy(const T* const str)
            {
                bool res;
                if( Parent::isConstructed() && str != NULL )
                {
                    int32 const len = Parent::getLength(str);
                    res = true;
                    // If a given string length is more than this max available length
                    if( not context_.isFit(len) )
                    {
                        // Create a new temporary string context
                        Context context;
                        if( context.allocate(len) )
                        {
                            // Delete this string context
                            context_.free();
                            // Set new contex
                            context_.mirror(context);
                        }
                        else
                        {
                            res = false;
                        }
                    }
                    else
                    {
                        context_.len = len;
                    }
                    if(res == true)
                    {
                        Parent::copy(context_.str, str);
                    }
                }
                else
                {
                    res = false;
                }
                return res;
            }

            /**
             * Concatenates a passed string to this string.
             *
             * @param str - an character string to be appended.
             * @return true if a passed string has been appended successfully.
             */
            virtual bool concatenate(const T* const str)
            {
                bool res;
                if( Parent::isConstructed() && str != NULL )
                {
                    // Simply, copy a given string if the context is freed
                    if( not context_.isAllocated() )
                    {
                        res = Self::copy(str);
                    }
                    else
                    {
                        res = true;
                        int32 const len = Parent::getLength(str) + context_.len;
                        // If a length of this string plus a given string is more than this max available length
                        if( not context_.isFit(len) )
                        {
                            // Create a new temporary string context
                            Context context;
                            if( context.allocate(len) )
                            {
                                // Copy a string of this context to the new contex string
                                Parent::copy(context.str, context_.str);
                                // Delete this string context
                                context_.free();
                                // Set new contex
                                context_.mirror(context);
                            }
                            else
                            {
                                res = false;
                            }
                        }
                        else
                        {
                            context_.len = len;
                        }
                        if(res == true)
                        {
                            Parent::concatenate(context_.str, str);
                        }
                    }
                }
                else
                {
                    res = false;
                }
                return res;
            }

            /**
             * Compares this string with a passed string lexicographically.
             *
             * @param str - a character string to be compared.
             * @return the value 0 if a passed string is equal to this string;
             *         a value less than 0 if this string is less than a passed string;
             *         a value greater than 0 if this string is greater than a passed string,
             *         or the minimum possible value if an error has been occurred.
             */
            virtual int32 compare(const T* const str) const
            {
                int32 res;
                if( Parent::isConstructed() && context_.str != NULL && str != NULL )
                {
                    res = context_.len - Parent::getLength(str);
                    // If lengths are equal, characters might be different
                    if(res == 0)
                    {
                        for(int32 i=0; i<context_.len; i++)
                        {
                            intptr const a1 = static_cast<intptr>(context_.str[i]);
                            intptr const a2 = static_cast<intptr>(str[i]);
                            res = static_cast<int32>(a1 - a2);
                            if(res != 0)
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    res = Parent::MINIMUM_POSSIBLE_VALUE_OF_INT32;
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
             * @param obj - a source object.
             */
            AbstractString(const AbstractString<T,L,A>& obj);

            /**
             * Direct assignment operator.
             *
             * The assignment to this object any values of another object is prohibited.
             * The operation is performed by calling the copy interface function,
             * because it does not deconstruct the object.
             *
             * @param obj - a source object.
             * @return this object.
             */
            AbstractString<T,L,A>& operator=(const AbstractString<T,L,A>& obj);

            /**
             * A contex of this class containing string.
             */
            struct Context
            {

            public:

                /**
                 * The first character of this string.
                 */
                T* str;

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
                 * @param obj - a source object.
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
                 * @param length - a number of string characters.
                 * @return true if the context has been allocated successfully.
                 */
                bool allocate(int32 const length)
                {
                    bool res;
                    if(str != NULL || length > L)
                    {
                        res = false;
                    }
                    else
                    {
                        // Set this class variables
                        str = buf_;
                        len = length;
                        max = L;
                        res = true;
                    }
                    return res;
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
                    bool res;
                    if( str == NULL )
                    {
                        res = false;
                    }
                    else
                    {
                        res = true;
                    }
                    return res;
                }

                /**
                 * Tests if a passed length fits to this max available length.
                 *
                 * @param len - a number of string characters.
                 * @return true if this length will be fit successfully.
                 */
                bool isFit(int32 len) const
                {
                    bool res;
                    if( len > max )
                    {
                        res = false;
                    }
                    else
                    {
                        res = true;
                    }
                    return res;
                }

            private:

                /**
                 * Constructor.
                 *
                 * @param obj - a source object.
                 */
                Context(const Context& context);

                /**
                 * Assignment operator.
                 *
                 * @param obj - a source object.
                 * @return this object.
                 */
                Context& operator=(const Context& obj);

                /**
                 * The buffer of characters of this string.
                 */
                T buf_[L + 1];

            };

            /**
             * A contex of this class containing string.
             */
            Context context_;

        };

        #ifdef EOOS_NO_STRICT_MISRA_RULES

        /**
         * Partial specialization of the template implements the dynamic string class.
         *
         * @param T - data type of string characters.
         * @param A - heap memory allocator class.
         */
        template <typename T, class A>
        class AbstractString<T,0,A> : public AbstractBaseString<T,A>
        {
            typedef library::AbstractString<T,0,A>    Self;
            typedef library::AbstractBaseString<T,A>  Parent;

        public:

            using Parent::copy;
            using Parent::concatenate;
            using Parent::compare;

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
                return Parent::isConstructed() ? context_.len : 0;
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
            virtual const T* getChar() const
            {
                return context_.str;
            }

        protected:

            /**
             * Copies a passed string into this string.
             *
             * @param str - a character string to be copied.
             * @return true if a passed string has been copied successfully.
             */
            virtual bool copy(const T* const str)
            {
                bool res;
                if( Parent::isConstructed() && str != NULL )
                {
                    int32 const len = Parent::getLength(str);
                    res = true;
                    // If a given string length is more than this max available length
                    if( not context_.isFit(len) )
                    {
                        // Create a new temporary string context
                        Context context;
                        if( context.allocate(len) )
                        {
                            // Delete this string context
                            context_.free();
                            // Set new contex
                            context_.mirror(context);
                        }
                        else
                        {
                            res = false;
                        }
                    }
                    else
                    {
                        context_.len = len;
                    }
                    if(res == true)
                    {
                        Parent::copy(context_.str, str);
                    }
                }
                else
                {
                    res = false;
                }
                return res;
            }

            /**
             * Concatenates a passed string to this string.
             *
             * @param str - an character string to be appended.
             * @return true if a passed string has been appended successfully.
             */
            virtual bool concatenate(const T* const str)
            {
                bool res;
                if( Parent::isConstructed() && str != NULL )
                {
                    // Simply, copy a given string if the context is freed
                    if( not context_.isAllocated() )
                    {
                        res = Self::copy(str);
                    }
                    else
                    {
                        res = true;
                        int32 const len = Parent::getLength(str) + context_.len;
                        // If a length of this string plus a given string is more than this max available length
                        if( not context_.isFit(len) )
                        {
                            // Create a new temporary string context
                            Context context;
                            if( context.allocate(len) )
                            {
                                // Copy a string of this context to the new contex string
                                Parent::copy(context.str, context_.str);
                                // Delete this string context
                                context_.free();
                                // Set new contex
                                context_.mirror(context);
                            }
                            else
                            {
                                res = false;
                            }
                        }
                        else
                        {
                            context_.len = len;
                        }
                        if(res == true)
                        {
                            Parent::concatenate(context_.str, str);
                        }
                    }
                }
                else
                {
                    res = false;
                }
                return res;
            }

            /**
             * Compares this string with a passed string lexicographically.
             *
             * @param str - a character string to be compared.
             * @return the value 0 if a passed string is equal to this string;
             *         a value less than 0 if this string is less than a passed string;
             *         a value greater than 0 if this string is greater than a passed string,
             *         or the minimum possible value if an error has been occurred.
             */
            virtual int32 compare(const T* const str) const
            {
                int32 res;
                if( Parent::isConstructed() && context_.str != NULL && str != NULL )
                {
                    res = context_.len - Parent::getLength(str);
                    // If lengths are equal, characters might be different
                    if(res == 0)
                    {
                        for(int32 i=0; i<context_.len; i++)
                        {
                            intptr const a1 = static_cast<intptr>(context_.str[i]);
                            intptr const a2 = static_cast<intptr>(str[i]);
                            res = static_cast<int32>(a1 - a2);
                            if(res != 0)
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    res = Parent::MINIMUM_POSSIBLE_VALUE_OF_INT32;
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
             * @param obj - a source object.
             */
            AbstractString(const AbstractString<T,0,A>& obj);

            /**
             * Direct assignment operator.
             *
             * The assignment to this object any values of another object is prohibited.
             * The operation is performed by calling the copy interface function,
             * because it does not deconstruct the object.
             *
             * @param obj - a source object.
             * @return this object.
             */
            AbstractString<T,0,A>& operator=(const AbstractString<T,0,A>& obj);

            /**
             * A contex of this class containing string.
             */
            struct Context
            {

            public:

                /**
                 * The first character of this string.
                 */
                T* str;

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
                 * @param obj - a source object.
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
                 * @param length - a number of string characters.
                 * @return true if the context has been allocated successfully.
                 */
                bool allocate(int32 const length)
                {
                    bool res;
                    if(str == NULL)
                    {
                        // Calculate size in byte for the given length
                        int32 const size = calculateSize(length);
                        // Allocate a new array
                        T* const string = reinterpret_cast<T*>( A::allocate(size) );
                        if(string == NULL)
                        {
                            res = false;
                        }
                        else
                        {
                            // Set this class variables
                            str = string;
                            len = length;
                            max = calculateLength(size);
                            res = true;
                        }
                    }
                    else
                    {
                        res = false;
                    }
                    return res;
                }

                /**
                 * Frees this contex.
                 */
                void free()
                {
                    if(str != NULL)
                    {
                        A::free(str);
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
                    bool res;
                    if( str == NULL )
                    {
                        res = false;
                    }
                    else
                    {
                        res = true;
                    }
                    return res;
                }

                /**
                 * Tests if a passed length fits to this max available length.
                 *
                 * @param len - a number of string characters.
                 * @return true if this length will be fit successfully.
                 */
                bool isFit(int32 len) const
                {
                    bool res;
                    if( len > max )
                    {
                        res = false;
                    }
                    else
                    {
                        res = true;
                    }
                    return res;
                }

            private:

                /**
                 * Returns size in byte for a string length.
                 *
                 * @param len - a number of string characters.
                 * @return size in byte for a passed string.
                 */
                static int32 calculateSize(int32 len)
                {
                    size_t size = static_cast<size_t>(len) * sizeof(T) + sizeof(T);
                    // Align size to eight
                    size_t const align = size & 0x7U;
                    if(align != 0U)
                    {
                        size_t const mask = 0x7U;
                        size = ( size & static_cast<size_t>(~mask) ) + 0x8U;
                    }
                    int32 const res = static_cast<int32>(size);
                    return res;
                }

                /**
                 * Returns a string length of size in byte.
                 *
                 * @param size - size in byte.
                 * @return a number of string characters.
                 */
                static int32 calculateLength(int32 const size)
                {
                    uint32 const bytes = sizeof(T);
                    int32 const len = size / static_cast<int32>(bytes);
                    return (len > 1) ? len - 1 : 0;
                }

                /**
                 * Constructor.
                 *
                 * @param obj - a source object.
                 */
                Context(const Context& context);

                /**
                 * Assignment operator.
                 *
                 * @param obj - a source object.
                 * @return this object.
                 */
                Context& operator=(const Context& obj);

            };

            /**
             * A contex of this class containing string.
             */
            Context context_;

        };

        #endif // EOOS_NO_STRICT_MISRA_RULES

    }
}
#endif // LIBRARY_ABSTRACT_STRING_HPP_
