/**
 * @file      lib.AbstractString.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ABSTRACTSTRING_HPP_
#define LIB_ABSTRACTSTRING_HPP_

#include "lib.AbstractBaseString.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class AbstractString<T,L,A>
 * @brief Static abstract string class.
 *
 * @tparam T A data type of string characters.
 * @tparam L A maximum number of string characters, or 0 for dynamic allocation.
 * @tparam A A heap memory allocator class.
 */
template <typename T, int32_t L, class A = Allocator>
class AbstractString : public AbstractBaseString<T,A>
{
    typedef AbstractString<T,L,A>    Self;
    typedef AbstractBaseString<T,A>  Parent;

public:

    using Parent::copy;
    using Parent::concatenate;
    using Parent::compare;

    /**
     * @brief Constructor.
     */
    AbstractString() 
        : AbstractBaseString<T,A>()
        , context_() {
    }

    /**
     * @brief Destructor.
     */
    virtual ~AbstractString()
    {
        context_.free();
    }

    /**
     * @copydoc eoos::api::Collection::getLength()
     */
    virtual int32_t getLength() const
    {
        return context_.len;
    }

    /**
     * @copydoc eoos::api::String::getLength()
     */
    virtual T const* getChar() const
    {
        return context_.str;
    }

protected:

    /**
     * @copydoc eoos::lib::AbstractBaseString::copy(T const*)
     */
    virtual bool_t copy(T const* const str)
    {
        bool_t res;
        if( Parent::isConstructed() && (str != NULLPTR) )
        {
            int32_t const len( Parent::getLength(str) );
            res = true;
            // If a given string length is more than this max available length
            if( !context_.isFit(len) )
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
     * @copydoc eoos::lib::AbstractBaseString::concatenate(T const*)
     */
    virtual bool_t concatenate(T const* const str)
    {
        bool_t res;
        if( Parent::isConstructed() && (str != NULLPTR) )
        {
            // Simply, copy a given string if the context is freed
            if( !context_.isAllocated() )
            {
                res = Self::copy(str);
            }
            else
            {
                res = true;
                int32_t const len( Parent::getLength(str) + context_.len );
                // If a length of this string plus a given string is more than this max available length
                if( !context_.isFit(len) )
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
     * @copydoc eoos::lib::AbstractBaseString::compare(T const*)
     */
    virtual int32_t compare(T const* const str) const
    {
        int32_t res;
        if( Parent::isConstructed() && (context_.str != NULLPTR) && (str != NULLPTR) )
        {
            res = context_.len - Parent::getLength(str);
            // If lengths are equal, characters might be different
            if(res == 0)
            {
                for(int32_t i(0); i<context_.len; i++)
                {
                    uintptr_t const a1( static_cast<uintptr_t>(context_.str[i]) );
                    uintptr_t const a2( static_cast<uintptr_t>(str[i]) );
                    res = static_cast<int32_t>(a1) - static_cast<int32_t>(a2);
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
     * @brief Constructor.
     *
     * @note Creating this object based on another object is prohibited.
     * The operation is performed by calling the default object constructor
     * and the copy interface function after that. This sequence of calls is
     * determined because the copy function does not deconstruct the object.
     *
     * @param obj A source object.
     */
    AbstractString(AbstractString<T,L,A> const& obj);

    /**
     * @brief Direct assignment operator.
     *
     * The assignment to this object any values of another object is prohibited.
     * The operation is performed by calling the copy interface function,
     * because it does not deconstruct the object.
     *
     * @param obj A source object.
     * @return This object.
     */
    AbstractString<T,L,A>& operator=(AbstractString<T,L,A> const& obj);

    /**
     * @struct Context
     * @brief A contex of this class containing string.
     */
    struct Context
    {

    public:

        /**
         * @brief The first character of this string.
         */
        T* str; ///< SCA MISRA-C++:2008 Justified Rule 11-0-1

        /**
         * @brief Current number of characters of this string.
         */
        int32_t len; ///< SCA MISRA-C++:2008 Justified Rule 11-0-1

        /**
         * @brief Constructor.
         */
        Context() :
            str  (NULLPTR),
            len  (0),
            max_ (0){
        }

        /**
         * @brief Destructor.
         */
       ~Context()
        {
        }

        /**
         * @brief Mirrors an context to this.
         *
         * @param obj A source object.
         */
        void mirror(Context const& obj)
        {
            // Don't instigate a copy of object buffer to this buffer,
            // as the source object always is temporary and
            // doesn't have any information in its buffer.
            str = (obj.str != NULLPTR) ? buf_ : NULLPTR;
            len = obj.len;
            max_ = obj.max_;
        }

        /**
         * @brief Allocates this contex.
         *
         * @param length A number of string characters.
         * @return True if the context has been allocated successfully.
         */
        bool_t allocate(int32_t const length)
        {
            bool_t res;
            if( (str != NULLPTR) || (length > L) )
            {
                res = false;
            }
            else
            {
                // Set this class variables
                str = buf_;
                len = length;
                max_ = L;
                res = true;
            }
            return res;
        }

        /**
         * @brief Frees this contex.
         */
        void free()
        {
            str = NULLPTR;
            len = 0;
            max_ = 0;
        }

        /**
         * @brief Test if this contex is allocated.
         *
         * @return True if the context has been allocated successfully.
         */
        bool_t isAllocated() const
        {
            bool_t res;
            if( str == NULLPTR )
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
         * @brief Tests if a passed length fits to this max available length.
         *
         * @param len A number of string characters.
         * @return True if this length will be fit successfully.
         */
        bool_t isFit(int32_t len) const ///< SCA MISRA-C++:2008 Justified Rule 2-10-2
        {
            bool_t res;
            if( len > max_ )
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
         * @brief Constructor.
         *
         * @param obj A source object.
         */
        Context(Context const& context);

        /**
         * @brief Assignment operator.
         *
         * @param obj A source object.
         * @return This object.
         */
        Context& operator=(Context const& obj);
        
        /**
         * @brief Max available number of characters for this string.
         */
        int32_t max_;        

        /**
         * @brief The buffer of characters of this string.
         */
        T buf_[L + 1];

    };

    /**
     * @brief A contex of this class containing string.
     */
    Context context_;

};

#ifdef EOOS_NO_STRICT_MISRA_RULES

/**
 * @class AbstractString<T,A>
 * @brief Dynamic abstract string class.
 *
 * @tparam T Data type of string characters.
 * @tparam A Heap memory allocator class.
 */
template <typename T, class A>
class AbstractString<T,0,A> : public AbstractBaseString<T,A>
{
    typedef AbstractString<T,0,A>    Self;
    typedef AbstractBaseString<T,A>  Parent;

public:

    using Parent::copy;
    using Parent::concatenate;
    using Parent::compare;

    /**
     * @brief Constructor.
     */
    AbstractString() : Parent(),
        context_ (){
    }

    /**
     * @brief Destructor.
     */
    virtual ~AbstractString()
    {
        context_.free();
    }

    /**
     * @copydoc eoos::api::Collection::getLength()
     */
    virtual int32_t getLength() const
    {
        return Parent::isConstructed() ? context_.len : 0;
    }

    /**
     * @copydoc eoos::api::String::getChar()    
     *
     * @note Be careful, some action with the object might relocate internal buffer
     * that contains characters. By this reason, a returned address will be actual
     * until you do not call no constant function of this class for an object.
     */
    virtual T const* getChar() const
    {
        return context_.str;
    }

protected:

    /**
     * @copydoc eoos::lib::AbstractBaseString::copy(T const*)
     */
    virtual bool_t copy(T const* const str)
    {
        bool_t res;
        if( Parent::isConstructed() && (str != NULLPTR) )
        {
            int32_t const len( Parent::getLength(str) );
            res = true;
            // If a given string length is more than this max available length
            if( !context_.isFit(len) )
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
     * @copydoc eoos::lib::AbstractBaseString::concatenate(T const*)
     */
    virtual bool_t concatenate(T const* const str)
    {
        bool_t res;
        if( Parent::isConstructed() && (str != NULLPTR) )
        {
            // Simply, copy a given string if the context is freed
            if( !context_.isAllocated() )
            {
                res = Self::copy(str);
            }
            else
            {
                res = true;
                int32_t const len( Parent::getLength(str) + context_.len );
                // If a length of this string plus a given string is more than this max available length
                if( !context_.isFit(len) )
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
     * @copydoc eoos::lib::AbstractBaseString::compare(T const*)
     */
    virtual int32_t compare(T const* const str) const
    {
        int32_t res;
        if( Parent::isConstructed() && (context_.str != NULLPTR) && (str != NULLPTR) )
        {
            res = context_.len - Parent::getLength(str);
            // If lengths are equal, characters might be different
            if(res == 0)
            {
                for(int32_t i(0); i<context_.len; i++)
                {
                    uintptr_t const a1( static_cast<uintptr_t>(context_.str[i]) );
                    uintptr_t const a2( static_cast<uintptr_t>(str[i]) );
                    res = static_cast<int32_t>(a1) - static_cast<int32_t>(a2);
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
     * @brief Constructor.
     *
     * @note Creating this object based on another object is prohibited.
     * The operation is performed by calling the default object constructor
     * and the copy interface function after that. This sequence of calls is
     * determined because the copy function does not deconstruct the object.
     *
     * @param obj A source object.
     */
    AbstractString(AbstractString<T,0,A> const& obj);

    /**
     * @brief Direct assignment operator.
     *
     * The assignment to this object any values of another object is prohibited.
     * The operation is performed by calling the copy interface function,
     * because it does not deconstruct the object.
     *
     * @param obj A source object.
     * @return This object.
     */
    AbstractString<T,0,A>& operator=(AbstractString<T,0,A> const& obj);

    /**
     * @struct Context
     * @brief A contex of this class containing string.
     */
    struct Context
    {

    public:

        /**
         * @brief The first character of this string.
         */
        T* str; ///< SCA MISRA-C++:2008 Justified Rule 11-0-1

        /**
         * @brief Current number of characters of this string.
         */
        int32_t len; ///< SCA MISRA-C++:2008 Justified Rule 11-0-1

        /**
         * @brief Constructor.
         */
        Context() :
            str  (NULLPTR),
            len  (0),
            max_ (0){
        }

        /**
         * @brief Destructor.
         */
       ~Context()
        {
        }

        /**
         * @brief Mirrors an context to this.
         *
         * @param obj A source object.
         */
        void mirror(Context const& obj)
        {
            // Copy an argument pointer value to this pointer value,
            // as we know that the source object will be destroyed
            // immediately after this function called without deleting
            // of the object allocated heap for the array of its characters.
            str = obj.str;
            len = obj.len;
            max_ = obj.max_;
        }

        /**
         * @brief Allocates this contex.
         *
         * @param length A number of string characters.
         * @return True if the context has been allocated successfully.
         */
        bool_t allocate(int32_t const length)
        {
            bool_t res;
            if(str == NULLPTR)
            {
                // Calculate size in byte for the given length
                int32_t const size( calculateSize(length) );
                // Allocate a new array
                T* const string( reinterpret_cast<T*>( A::allocate(size) ) );
                if(string == NULLPTR)
                {
                    res = false;
                }
                else
                {
                    // Set this class variables
                    str = string;
                    len = length;
                    max_ = calculateLength(size);
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
         * @brief Frees this contex.
         */
        void free()
        {
            if(str != NULLPTR)
            {
                A::free(str);
                str = NULLPTR;
                len = 0;
                max_ = 0;
            }
        }

        /**
         * @brief Test if this contex is allocated.
         *
         * @return True if the context has been allocated successfully.
         */
        bool_t isAllocated()
        {
            bool_t res;
            if( str == NULLPTR )
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
         * @brief Tests if a passed length fits to this max available length.
         *
         * @param len A number of string characters.
         * @return True if this length will be fit successfully.
         */
        bool_t isFit(int32_t len) const ///< SCA MISRA-C++:2008 Justified Rule 2-10-2
        {
            bool_t res;
            if( len > max_ )
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
         * @brief Returns size in byte for a string length.
         *
         * @param len A number of string characters.
         * @return Size in byte for a passed string.
         */
        static int32_t calculateSize(int32_t len)
        {
            size_t size( (static_cast<size_t>(len) * sizeof(T)) + sizeof(T) );
            // Align size to eight
            size_t const align( size & 0x7U );
            if(align != 0U)
            {
                size_t const mask( 0x7U );
                size = ( size & static_cast<size_t>(~mask) ) + 0x8U;
            }
            int32_t const res( static_cast<int32_t>(size) );
            return res;
        }

        /**
         * @brief Returns a string length of size in byte.
         *
         * @param size Size in byte.
         * @return A number of string characters.
         */
        static int32_t calculateLength(int32_t const size)
        {
            uint32_t const bytes( sizeof(T) );
            int32_t const len( size / static_cast<int32_t>(bytes) );
            return (len > 1) ? (len - 1) : 0;
        }

        /**
         * @brief Constructor.
         *
         * @param obj A source object.
         */
        Context(Context const& context);

        /**
         * @brief Assignment operator.
         *
         * @param obj A source object.
         * @return This object.
         */
        Context& operator=(Context const& obj);
        
        /**
         * @brief Max available number of characters for this string.
         */
        int32_t max_;        

    };

    /**
     * @brief A contex of this class containing string.
     */
    Context context_;

};

#endif // EOOS_NO_STRICT_MISRA_RULES

} // namespace lib
} // namespace eoos
#endif // LIB_ABSTRACTSTRING_HPP_
