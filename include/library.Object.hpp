/** 
 * Root class of the library class hierarchy.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2018, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_OBJECT_HPP_
#define LIBRARY_OBJECT_HPP_

#include "Object.hpp"

namespace global
{
    namespace library
    {
        /** 
         * Primary template implementation.
         *
         * @param A - heap memory allocator class.
         */     
        template <class A = Allocator>    
        class Object : public ::global::Object<A>
        {
            typedef ::global::Object<Allocator> Parent;        
        
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
