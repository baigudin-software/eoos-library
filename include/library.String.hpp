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

namespace library
{
    /** 
     * Primary template.
     *
     * @param Type  data type of string characters.
     * @param Alloc heap memory allocator class.
     */
    template <typename Type, class Alloc = Allocator>    
    class String : public ::library::AbstractString<Type,Alloc>
    {
        typedef ::library::AbstractString<Type,Alloc> Parent;

    public:
    
        /** 
         * Constructor.
         *
         * @param string a character string to be set.         
         */    
        String(const Type* string) : Parent(string)
        {
        }     
    
    protected:

        /**
         * Returns this string terminated character.
         *
         * @return a character which means that this string terminated.
         */         
        virtual Type getTerminatedChar() const
        {
            return '\0';
        }
    
    };
}
#endif // LIBRARY_STRING_HPP_
