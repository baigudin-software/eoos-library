/**
 * Alignment of simple types to byte boundary of memory.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2018, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_ALIGN_HPP_
#define LIBRARY_ALIGN_HPP_

#include "Allocator.hpp"

namespace local
{
    namespace library
    {
        /**
         * Primary template implementation.
         *
         * MISRA C++ NOTE: Any signed underlying types shall not be used
         * for not violating the 5-0-21 MISRA C++:2008 Rule.
         *
         * @param T type of aligning data.
         * @param S size of aligning data type.
         * @param A heap memory allocator class.
         */
        template <typename T, int32 S = sizeof(T), class A = Allocator>
        class Align
        {

        public:

            /**
             * Constructor.
             */
            Align()
            {
                assignment(0);
            }

            /**
             * Constructor.
             *
             * NOTE: A passed value is copied to an internal data structure
             * so that the value might be invalidated after the function called.
             *
             * @param value a data value.
             */
            Align(const T& value)
            {
                assignment(value);
            }

            /**
             * Copy constructor.
             *
             * @param obj a source object.
             */
            Align(const Align& obj)
            {
                copy(obj);
            }

            /**
             * Compares this string with a passed string lexicographically.
             *
             * @param val a source value.
             * @return true if this object value equals to a passed value.
             */
            bool equal(const T& value) const
            {
                Align<T,S,A> obj(value);
                return equal(obj);
            }

            /**
             * Compares this string with a passed string lexicographically.
             *
             * @param obj a source object.
             * @return true if this object value equals to a passed object value.
             */
            bool equal(const Align& obj) const
            {
                bool res = true;
                for(int32 i=0; i<SIZE; i++)
                {
                    if( val_[i] != obj.val_[i] )
                    {
                        res = false;
                        break;
                    }
                }
                return res;
            }

            /**
             * Assignment operator.
             *
             * NOTE: A passed value is copied to an internal data structure
             * so that the value might be invalidated after the function called.
             *
             * @param value a source data value.
             * @return reference to this object.
             */
            Align& operator=(const T& value)
            {
                assignment(value);
                return *this;
            }

            /**
             * Assignment operator.
             *
             * @param obj a source object.
             * @return reference to this object.
             */
            Align& operator=(const Align& obj)
            {
                copy(obj);
                return *this;
            }

            /**
             * Casts to the template data type.
             *
             * @return a data value.
             */
            operator T() const
            {
                return typecast();
            }

            #ifdef EOOS_NO_STRICT_MISRA_RULES

            /**
             * Operator new.
             *
             * @param size number of bytes to allocate.
             * @return allocated memory address or a null pointer.
             */
            void* operator new(const size_t size)
            {
                return A::allocate(size);
            }

            /**
             * Operator new.
             *
             * @param size unused.
             * @param ptr  pointer to reserved memory area
             * @return given pointer.
             */
            void* operator new(size_t, void* const ptr)
            {
                return ptr;
            }

            /**
             * Operator delete.
             *
             * @param ptr address of allocated memory block or a null pointer.
             */
            void operator delete(void* const ptr)
            {
                A::free(ptr);
            }

            #endif // EOOS_NO_STRICT_MISRA_RULES

        private:

            /**
             * Assigns given value to self data.
             *
             * @param value source data value.
             */
            void assignment(const T& value)
            {
                for(int32 i = 0; i<SIZE; i++)
                {
                    const T v = value >> (8 * i);
                    val_[i] = static_cast<cell>(v);
                }
            }

            /**
             * Copies given object to self data.
             *
             * @param obj reference to source object.
             */
            void copy(const Align& obj)
            {
                for(register int32 i=0; i<SIZE; i++)
                {
                    val_[i] = obj.val_[i];
                }
            }

            /**
             * Returns conversed data to type of aligning data.
             *
             * @return conversed data.
             */
            T typecast() const
            {
                T r = static_cast<T>(0);
                for(int32 i=SIZE-1; i>=0; i--)
                {
                    r = r << 8;
                    r = r | static_cast<T>(val_[i]);
                }
                return r;
            }

            /**
             * Size of aligning data.
             */
            static const int32 SIZE = S;

            /**
             * Array of data bytes.
             */
            cell val_[SIZE];

        };

        /**
         * Comparison operator to equal.
         *
         * @param obj1 reference to object.
         * @param obj2 reference to object.
         * @return true if objects are equal.
         */
        template <typename T>
        inline bool operator==(const library::Align<T>& obj1, const library::Align<T>& obj2)
        {
            const bool res = obj1.equal(obj2);
            return res;
        }

        /**
         * Comparison operator to unequal.
         *
         * @param obj1 reference to object.
         * @param obj2 reference to object.
         * @return true if objects are not equal.
         */
        template <typename T>
        inline bool operator!=(const library::Align<T>& obj1, const library::Align<T>& obj2)
        {
            const bool res = obj1.equal(obj2);
            return not res;
        }
    }
}
#endif // LIBRARY_ALIGN_HPP_
