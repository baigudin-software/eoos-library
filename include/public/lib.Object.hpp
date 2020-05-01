/**
 * Root class of the library class hierarchy.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_OBJECT_HPP_
#define LIB_OBJECT_HPP_

#include "Object.hpp"

namespace eoos
{
    namespace lib
    {
        /**
         * Primary template implementation.
         *
         * @param A - heap memory allocator class.
         */
        template <class A = Allocator>
        class Object : public ::eoos::Object<A>
        {
            typedef ::eoos::Object<A> Parent;

        public:

            /**
             * Constructor.
             */
            Object() : Parent()
            {
            }

            /**
             * Destructor.
             */
            virtual ~Object()
            {
            }

        };
    }
}
#endif // LIB_OBJECT_HPP_
