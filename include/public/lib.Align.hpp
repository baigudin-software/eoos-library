/**
 * Alignment of simple types to byte boundary of memory.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2018, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ALIGN_HPP_
#define LIB_ALIGN_HPP_

#include "Allocator.hpp"

namespace eoos
{
    namespace lib
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
        template <typename T, size_t S = sizeof(T), class A = Allocator>
        class Align
        {

        public:

            /**
             * Constructor.
             */
            Align()
            {
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
             * Pre-increment operators.
             *
             * @param obj a source object.
             * @return reference to this object.
             */
            Align& operator++()
            {
                T val = typecast();
                assignment(++val);
                return *this;
            }

            /**
             * Pre-decrement operators.
             *
             * @param obj a source object.
             * @return reference to this object.
             */
            Align& operator--()
            {
                T val = typecast();
                assignment(--val);
                return *this;
            }

            /**
             * Post-increment operators.
             *
             * @return reference to this object.
             */
            Align operator++(int)
            {
                T val = typecast();
                assignment(val++);
                return *this;
            }

            /**
             * Post-decrement operators.
             *
             * @return reference to this object.
             */
            Align operator--(int)
            {
                T val = typecast();
                assignment(val--);
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
             * Compares a type based value with this class.
             *
             * @param val a source value.
             * @return true if this object value equals to a passed value.
             */
            bool_t equal(const T& value) const
            {
                Align<T,S,A> obj(value);
                return equal(obj);
            }

            /**
             * Compares an object of this class type with this class.
             *
             * @param obj a source object.
             * @return true if this object value equals to a passed object value.
             */
            bool_t equal(const Align& obj) const
            {
                bool_t res = true;
                for(size_t i=0; i<SIZE; i++)
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
             * Assigns given value to self data.
             *
             * @param value source data value.
             */
            void assignment(const T& value)
            {
                for(size_t i = 0; i<SIZE; i++)
                {
                    const T v = value >> (8 * i);
                    val_[i] = static_cast<cell_t>(v);
                }
            }

            /**
             * Copies given object to self data.
             *
             * @param obj reference to source object.
             */
            void copy(const Align& obj)
            {
                for(size_t i=0; i<SIZE; i++)
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
                for(size_t i=SIZE-1; i>=0; i--)
                {
                    r = r << 8;
                    r = r | static_cast<T>(val_[i]);
                }
                return r;
            }

            /**
             * Size of aligning data.
             */
            static const size_t SIZE = S;

            /**
             * Array of data bytes.
             */
            cell_t val_[SIZE];

        };

        /**
         * Comparison operator to equal.
         *
         * @param obj1 reference to object.
         * @param obj2 reference to object.
         * @return true if objects are equal.
         */
        template <typename T>
        inline bool_t operator==(const Align<T>& obj1, const Align<T>& obj2)
        {
            const bool_t res = obj1.equal(obj2);
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
        inline bool_t operator!=(const Align<T>& obj1, const Align<T>& obj2)
        {
            const bool_t res = obj1.equal(obj2);
            return not res;
        }
    }
}
#endif // LIB_ALIGN_HPP_
