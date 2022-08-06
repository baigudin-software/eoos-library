/**
 * @file      lib.BaseString.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_BASESTRING_HPP_
#define LIB_BASESTRING_HPP_

#include "lib.AbstractBaseString.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class BaseString<T,L,R,A>
 * @brief Static base string class.
 *
 * Primary template implements the static string class.
 *
 * @tparam T A data type of string characters.
 * @tparam L A maximum number of string characters, or 0 for dynamic allocation.
 * @tparam R A character traits.
 * @tparam A A heap memory allocator class.
 */
template <typename T, int32_t L, class R = CharTrait<T>, class A = Allocator>
class BaseString : public AbstractBaseString<T,R,A>
{
    typedef AbstractBaseString<T,R,A> Parent;
    
public:

    /**
     * @brief Constructor.
     */
    BaseString() 
        : AbstractBaseString<T,R,A>() {
        bool_t const isConstructed( construct() );
        setConstructed( isConstructed );
    }
    
    /**
     * @brief Constructor.
     *
     * @param source A source object interface.
     */
    BaseString(api::String<T> const& source) 
        : AbstractBaseString<T,R,A>() {
        bool_t const isConstructed( construct( source.getChar() ) );
        setConstructed( isConstructed );            
    }

    /**
     * @brief Constructor.
     *
     * @param source A source character string.
     */
    BaseString(T const* const source) 
        : AbstractBaseString<T,R,A>() {
        bool_t const isConstructed( construct( source ) );
        setConstructed( isConstructed );            
    }
    
    /**
     * @brief Constructor.
     *
     * @param value A source numerical value.
     * @param base  A numerical base used to represent a value as this string.     
     */
    explicit BaseString(int32_t const value, Number::Base const base = Number::BASE_10) 
        : AbstractBaseString<T,R,A>() {    
        bool_t isConstructed( construct() );
        if( isConstructed )
        {
            isConstructed = convert(value, base);
        }
        setConstructed( isConstructed );
    }

    /**
     * @brief Destructor.
     */
    virtual ~BaseString()
    {
    }
    
    /**
     * @copydoc eoos::Object::Object(Object const&)
     */
    BaseString(BaseString const& obj)
        : AbstractBaseString<T,R,A>(obj) {
        bool_t const isConstructed( construct( obj.getChar() ) );
        setConstructed( isConstructed );
    }

    /**
     * @copydoc eoos::Object::operator=(Object const&)
     */       
    BaseString& operator=(BaseString const& obj)
    {
        if( isConstructed() && (this != &obj) )
        {
            copyRaw3(str_, obj.str_, L);
            Parent::operator=(obj);            
        }
        return *this;
    }    

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(Object&&)
     */       
    BaseString(BaseString&& obj) noexcept 
        : AbstractBaseString<T,R,A>( move(obj) ) {
        copyRaw3(str_, obj.str_, L);
    }

    /**
     * @copydoc eoos::Object::operator=(Object&&)
     */
    BaseString& operator=(BaseString&& obj) & noexcept
    {
        if( isConstructed() && (this != &obj) )
        {
            copyRaw3(str_, obj.str_, L);
            Parent::operator=( move(obj) );            
        }
        return *this;
    }        

    #endif // EOOS_CPP_STANDARD >= 2011    

    /**
     * @copydoc eoos::api::Collection::getLength()
     */
    virtual size_t getLength() const
    {
        size_t length( 0 );
        if( isConstructed() )
        {
            // @todo Rework here not to calculate length, but take it form a class member.
            length = getLengthRaw(str_);
        }
        return length;
    }

    /**
     * @copydoc eoos::api::String::getChar()
     */
    virtual T const* getChar() const
    {
        T const* str( NULLPTR );
        if( isConstructed() )
        {
            str = str_;
        }
        return str;
    }

private:

    /**
     * @copydoc eoos::lib::AbstractBaseString::copyRaw(T const*)
     */
    virtual bool_t copyRaw(T const* const str)
    {
        bool_t res( false );
        if( isConstructed() && (str != NULLPTR) )
        {
            copyRaw3(str_, str, L);
            res = true;
        }
        return res;
    }
    
    /**
     * @copydoc eoos::lib::AbstractBaseString::concatenate(T const*)
     */
    virtual bool_t concatenateRaw(T const* const str)
    {
        bool_t res( false );
        if( isConstructed() && (str != NULLPTR) )
        {
            concatenateRaw3(str_, str, L);
            res = true;
        }
        return res;

    }
    
    /**
     * @copydoc eoos::lib::AbstractBaseString::isEqualToRaw(T const*)
     */
    virtual bool_t isEqualToRaw(T const* const str) const
    {
        bool_t res( false );
        if( isConstructed() && (str != NULLPTR) )
        {
            res = isEqualRaw2(str_, str);
        }
        return res;

    }
    
    /**
     * @brief Constructs this object.
     *
     * @param str A string to be copied on construction.
     * @return True if this object has been constructed successfully.
     */     
    bool_t construct(T const* const str = NULLPTR)
    {
        str_[0] = R::getTerminator();
        bool_t res( false );
        do
        {
            if( !isConstructed() )
            {
                break;
            }
            res = ( str == NULLPTR ) ? true : copyRaw(str);            
        } while(false);
        return res;
    }
    

    /**
     * @brief The buffer of characters of this string.
     */
    T str_[L + 1];    

};

#ifdef EOOS_ENABLE_DYNAMIC_HEAP_MEMORY

/**
 * @class BaseString<T,0,R,A>
 * @brief Dynamic base string class.
 *
 * Partial specialization of the template implements the dynamic string class.
 *
 * @tparam T Data type of string characters.
 * @tparam R A character traits. 
 * @tparam A Heap memory allocator class.
 */
template <typename T, class R, class A>
class BaseString<T,0,R,A> : public AbstractBaseString<T,R,A>
{
    typedef AbstractBaseString<T,R,A> Parent;

public:

    /**
     * @brief Constructor.
     */
    BaseString() 
        : AbstractBaseString<T,R,A>()
        , len_(0)
        , str_(NULLPTR){
        bool_t const isConstructed( construct() );
        setConstructed( isConstructed );
    }
    
    /**
     * @brief Constructor.
     *
     * @param source A source object interface.
     */
    BaseString(api::String<T> const& source) 
        : AbstractBaseString<T,R,A>()
        , len_(0)
        , str_(NULLPTR){
        bool_t const isConstructed( construct( source.getChar() ) );
        setConstructed( isConstructed );
    }

    /**
     * @brief Constructor.
     *
     * @param source A source character string.
     */
    BaseString(T const* const source) 
        : AbstractBaseString<T,R,A>()
        , len_(0)
        , str_(NULLPTR){
        bool_t const isConstructed( construct( source ) );
        setConstructed( isConstructed );
    }
    
    /**
     * @brief Constructor.
     *
     * @param value A source numerical value.     
     * @param base  A numerical base used to represent a value as this string.
     */
    explicit BaseString(int32_t const value, Number::Base const base = Number::BASE_10)
        : AbstractBaseString<T,R,A>()
        , len_(0)
        , str_(NULLPTR){
        bool_t isConstructed( construct() );
        if( isConstructed )
        {
            isConstructed = convert(value, base);
        }
        setConstructed( isConstructed );
    }    

    /**
     * @brief Destructor.
     */
    virtual ~BaseString()
    {
        free();
    }

    /**
     * @copydoc eoos::Object::Object(Object const&)
     */
    BaseString(BaseString const& obj)
        : AbstractBaseString<T,R,A>(obj)
        , len_(0)
        , str_(NULLPTR){
        bool_t const isConstructed( construct( obj.getChar() ) );
        setConstructed( isConstructed );
    }

    /**
     * @copydoc eoos::Object::operator=(Object const&)
     */       
    BaseString& operator=(BaseString const& obj)
    {
        if( isConstructed() && (this != &obj) )
        {
            static_cast<void>( copyRaw( obj.getChar() ) );
            Parent::operator=(obj);
        }
        return *this;
    }    

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(Object&&)
     */       
    BaseString(BaseString&& obj) noexcept 
        : AbstractBaseString<T,R,A>( move(obj) )
        , len_(obj.len_)
        , str_(obj.str_){
        obj.clean();
    }

    /**
     * @copydoc eoos::Object::operator=(Object&&)
     */
    BaseString& operator=(BaseString&& obj) & noexcept
    {
        if( isConstructed() && (this != &obj) )
        {
            free();
            len_ = obj.len_;
            str_ = obj.str_;
            obj.clean();
            Parent::operator=( move(obj) );            
        }
        return *this;
    }        

    #endif // EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::api::Collection::getLength()
     */
    virtual size_t getLength() const
    {
        size_t length( 0 );
        if( isConstructed() )
        {
            // @todo Rework here not to calculate length, but take it form a class member.
            length = getLengthRaw(str_);
        }
        return length;
    }

    /**
     * @copydoc eoos::api::String::getChar()
     */
    virtual T const* getChar() const
    {
        T const* str( NULLPTR );
        if( isConstructed() )
        {
            str = str_;
        }
        return str;
    }

private:

    /**
     * @copydoc eoos::lib::AbstractBaseString::copyRaw(T const*)
     */
    virtual bool_t copyRaw(T const* const str)
    {
        bool_t res( false );
        if( isConstructed() && (str != NULLPTR) )
        {
            size_t const length( getLengthRaw(str) );            
            bool_t isPrepared( prepareCopy(length) );
            if( isPrepared )
            {
                copyRaw3(str_, str, len_);
                res = true;
            }
        }
        return res;
    }
    
    /**
     * @copydoc eoos::lib::AbstractBaseString::concatenate(T const*)
     */
    virtual bool_t concatenateRaw(T const* const str)
    {
        bool_t res( false );
        if( isConstructed() && (str != NULLPTR) )
        {
            size_t const length( getLength() + getLengthRaw(str) );
            bool_t isPrepared( prepareConcatenate(length) );
            if( isPrepared )
            {
                concatenateRaw3(str_, str, len_);
                res = true;
            }
        }
        return res;

    }
    
    /**
     * @copydoc eoos::lib::AbstractBaseString::isEqualToRaw(T const*)
     */
    virtual bool_t isEqualToRaw(T const* const str) const
    {
        bool_t res( false );
        if( isConstructed() && (str != NULLPTR) )
        {
            res = isEqualRaw2(str_, str);
        }
        return res;

    }

    /**
     * @brief Constructs this object.
     *
     * @param str A string to be copied on construction.
     * @return True if this object has been constructed successfully.
     */     
    bool_t construct(T const* const str = NULLPTR)
    {
        bool_t res( false );
        do
        {
            if( !isConstructed() )
            {
                break;
            }
            size_t length( LENGTH_ON_CONSTRUCTION );
            if( str != NULLPTR )
            {
                length = getLengthRaw(str);
            }
            bool_t isAllocated( allocate(length) );
            if( !isAllocated )
            {
                break;
            }
            res = ( str == NULLPTR ) ? true : copyRaw(str);
        } while(false);
        return res;
    }
    
    /**
     * @brief Prepare this string for a new length to copy.
     *
     * @param length A number of string characters.
     * @return True if the the string is prepared.
     */
    bool_t prepareCopy(size_t length)
    {
        bool_t res(true);
        if( !isFit(length) )
        {
            free();
            res = allocate(length);
        }
        return res;
    }

    /**
     * @brief Prepare this string for a new length to copy.
     *
     * @param length A number of string characters.
     * @return True if the the string is prepared.
     */
    bool_t prepareConcatenate(size_t length)
    {
        bool_t res(true);
        if( !isFit(length) )
        {
            // @todo Refactor this implementation to make it be more clear.
            T* const str( str_ );
            size_t const len( len_ );            
            clean();
            res = allocate(length);
            if( res )
            {
                copyRaw3(str_, str, len);
            }
            A::free(str);
        }
        return res;
    }
    
    /**
     * @brief Allocates memory for a string.
     *
     * @param length A number of string characters.
     * @return True if the context has been allocated successfully.
     */
    bool_t allocate(int32_t const length)
    {
        bool_t res( false );
        if(str_ == NULLPTR && length != 0)
        {
            size_t const size( calculateSize(length) );
            T* const string( reinterpret_cast<T*>( A::allocate(size) ) );
            if(string != NULLPTR)
            {
                str_ = string;
                len_ = length;
                str_[0] = R::getTerminator();                
                res = true;
            }
        }
        return res;
    } 
    
    /**
     * @brief Frees this contex.
     */
    void free()
    {
        if(str_ != NULLPTR)
        {
            A::free(str_);
            str_ = NULLPTR;
            len_ = 0;
        }
    }
    
    /**
     * @brief Cleans this contex.
     */
    void clean()
    {
        str_ = NULLPTR;
        len_ = 0;
    }
    
    /**
     * @brief Returns size in byte for a string length.
     *
     * @param len A number of string characters.
     * @return Size in byte for a passed string.
     */
    static size_t calculateSize(size_t len)
    {
        return len * sizeof(T) + sizeof(T);
    }
    
    /**
     * @brief Tests if a passed length fits to allocated available length.
     *
     * @param len A number of string characters.
     * @return True if this length will be fit successfully.
     */
    bool_t isFit(size_t len) const ///< SCA MISRA-C++:2008 Justified Rule 2-10-2
    {
        return len <= len_;
    }
    
    /**
     * @brief Lenght of the buffer of characters on construction.
     *
     * @note Cannot be zero.
     */
    static const size_t LENGTH_ON_CONSTRUCTION = 7;
    
    /**
     * @brief Lenght of the buffer of characters of this string.
     */
    size_t len_;

    /**
     * @brief The buffer of characters of this string.
     *
     * @todo Refactor this to be array on construction, 
     * and if new assigned string is more than LENGTH_ON_CONSTRUCTION,
     * allocate a new array dynamically. To implement this approach and
     * reduce size of the class object, revise using a `union` type.
     */
    T* str_;
};

#endif // EOOS_ENABLE_DYNAMIC_HEAP_MEMORY

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object 2.
 * @return This string object with the concatenated strings.
 */
template <typename T, int32_t L, class R, class A>
inline BaseString<T,L,R,A> operator+(BaseString<T,L,R,A> const& source1, BaseString<T,L,R,A> const& source2)
{
    BaseString<T,L,R,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source object interface 2.
 * @return This string object with the concatenated strings.
 */
template <typename T, int32_t L, class R, class A>
inline BaseString<T,L,R,A> operator+(BaseString<T,L,R,A> const& source1, api::String<T> const& source2)
{
    BaseString<T,L,R,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source object interface 1.
 * @param source2 A source object 2.
 * @return This string object with the concatenated strings.
 */
template <typename T, int32_t L, class R, class A>
inline BaseString<T,L,R,A> operator+(api::String<T> const& source1, BaseString<T,L,R,A> const& source2)
{
    BaseString<T,L,R,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source object 1.
 * @param source2 A source character string 2.
 * @return This string object with the concatenated strings.
 */
template <typename T, int32_t L, class R, class A>
inline BaseString<T,L,R,A> operator+(BaseString<T,L,R,A> const& source1, T const* const source2)
{
    BaseString<T,L,R,A> string(source1);
    string += source2;
    return string;
}

/**
 * @brief Concatenates two strings.
 *
 * @param source1 A source character string 1.
 * @param source2 A source object 2.
 * @return This string object with the concatenated strings.
 */
template <typename T, int32_t L, class R, class A>
inline BaseString<T,L,R,A> operator+(T const* const source1, BaseString<T,L,R,A> const& source2)
{
    BaseString<T,L,R,A> string(source1);
    string += source2;
    return string;
}

} // namespace lib
} // namespace eoos
#endif // LIB_BASESTRING_HPP_
