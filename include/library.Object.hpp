/**
 * Root class of the library class hierarchy.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2018, Sergey Baigudin, Baigudin Software
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_OBJECT_HPP_
#define LIBRARY_OBJECT_HPP_

#include "Object.hpp"

namespace local
{
    namespace library
    {
        /**
         * Primary template implementation.
         *
         * @param A - heap memory allocator class.
         */
        template <class A = Allocator>
        class Object : public ::local::Object<A>
        {
            typedef ::local::Object<A> Parent;

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
#endif // LIBRARY_OBJECT_HPP_
