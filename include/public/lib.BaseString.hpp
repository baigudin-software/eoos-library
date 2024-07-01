/**
 * @file      lib.BaseString.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017-2024, Sergey Baigudin, Baigudin Software
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

    using Parent::isConstructed;
    using Parent::convert;

    /**
     * @brief Constructor.
     */
    BaseString();
    
    /**
     * @brief Constructor.
     *
     * @param source A source object interface.
     */
    BaseString(api::String<T> const& source);

    /**
     * @brief Constructor.
     *
     * @param source A source character string.
     */
    BaseString(T const* const source);
    
    /**
     * @brief Constructor.
     *
     * @param value A source numerical value.
     * @param base  A numerical base used to represent a value as this string.     
     */
    explicit BaseString(int32_t const value, Number::Base const base = Number::BASE_10);

    /**
     * @brief Destructor.
     */
    virtual ~BaseString();

    /**
     * @copydoc eoos::Object::Object(Object const&)
     */
    BaseString(BaseString const& obj);

    /**
     * @copydoc eoos::Object::operator=(Object const&)
     */       
    BaseString& operator=(BaseString const& obj);

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(Object&&)
     */       
    BaseString(BaseString&& obj) noexcept;

    /**
     * @copydoc eoos::Object::operator=(Object&&)
     */
    BaseString& operator=(BaseString&& obj) & noexcept;

    #endif // EOOS_CPP_STANDARD >= 2011    

    /**
     * @copydoc eoos::api::Collection::getLength()
     */
    virtual size_t getLength() const;

    /**
     * @copydoc eoos::api::String::getChar()
     */
    virtual T const* getChar() const;
    
protected:

    using Parent::setConstructed;
    using Parent::getLengthRaw;
    using Parent::copyRaw3;
    using Parent::concatenateRaw3;
    using Parent::isEqualRaw2;    

private:

    /**
     * @copydoc eoos::lib::AbstractBaseString::copyRaw(T const*)
     */
    virtual bool_t copyRaw(T const* const str);
    
    /**
     * @copydoc eoos::lib::AbstractBaseString::concatenate(T const*)
     */
    virtual bool_t concatenateRaw(T const* const str);
    
    /**
     * @copydoc eoos::lib::AbstractBaseString::isEqualToRaw(T const*)
     */
    virtual bool_t isEqualToRaw(T const* const str) const;
    
    /**
     * @brief Constructs this object.
     *
     * @param str A string to be copied on construction.
     * @return True if this object has been constructed successfully.
     */     
    bool_t construct(T const* const str = NULLPTR);

    /**
     * @brief The buffer of characters of this string.
     */
    T str_[L + 1];    

};

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

    using Parent::isConstructed;
    using Parent::convert;

    /**
     * @brief Constructor.
     */
    BaseString();
    
    /**
     * @brief Constructor.
     *
     * @param source A source object interface.
     */
    BaseString(api::String<T> const& source);

    /**
     * @brief Constructor.
     *
     * @param source A source character string.
     */
    BaseString(T const* const source);
    
    /**
     * @brief Constructor.
     *
     * @param value A source numerical value.     
     * @param base  A numerical base used to represent a value as this string.
     */
    explicit BaseString(int32_t const value, Number::Base const base = Number::BASE_10);

    /**
     * @brief Destructor.
     */
    virtual ~BaseString();

    /**
     * @copydoc eoos::Object::Object(Object const&)
     */
    BaseString(BaseString const& obj);

    /**
     * @copydoc eoos::Object::operator=(Object const&)
     */       
    BaseString& operator=(BaseString const& obj);

    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(Object&&)
     */       
    BaseString(BaseString&& obj) noexcept;

    /**
     * @copydoc eoos::Object::operator=(Object&&)
     */
    BaseString& operator=(BaseString&& obj) & noexcept;

    #endif // EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::api::Collection::getLength()
     */
    virtual size_t getLength() const;

    /**
     * @copydoc eoos::api::String::getChar()
     */
    virtual T const* getChar() const;

protected:

    using Parent::setConstructed;
    using Parent::getLengthRaw;
    using Parent::copyRaw3;
    using Parent::concatenateRaw3;
    using Parent::isEqualRaw2;

private:

    /**
     * @copydoc eoos::lib::AbstractBaseString::copyRaw(T const*)
     */
    virtual bool_t copyRaw(T const* const str);
    
    /**
     * @copydoc eoos::lib::AbstractBaseString::concatenate(T const*)
     */
    virtual bool_t concatenateRaw(T const* const str);
    
    /**
     * @copydoc eoos::lib::AbstractBaseString::isEqualToRaw(T const*)
     */
    virtual bool_t isEqualToRaw(T const* const str) const;

    /**
     * @brief Constructs this object.
     *
     * @param str A string to be copied on construction.
     * @return True if this object has been constructed successfully.
     */     
    bool_t construct(T const* const str = NULLPTR);
    
    /**
     * @brief Prepare this string for a new length to copy.
     *
     * @param length A number of string characters.
     * @return True if the the string is prepared.
     */
    bool_t prepareCopy(size_t length);

    /**
     * @brief Prepare this string for a new length to copy.
     *
     * @param length A number of string characters.
     * @return True if the the string is prepared.
     */
    bool_t prepareConcatenate(size_t length);
    
    /**
     * @brief Allocates memory for a string.
     *
     * @param length A number of string characters.
     * @return True if the context has been allocated successfully.
     */
    bool_t allocate(size_t const length);
    
    /**
     * @brief Frees this contex.
     */
    void free();
    
    /**
     * @brief Cleans this contex.
     */
    void clean();
    
    /**
     * @brief Returns size in byte for a string length.
     *
     * @param len A number of string characters.
     * @return Size in byte for a passed string.
     */
    static size_t calculateSize(size_t len);
    
    /**
     * @brief Tests if a passed length fits to allocated available length.
     *
     * @param len A number of string characters.
     * @return True if this length will be fit successfully.
     */
    bool_t isFit(size_t len) const;
    
    /**
     * @brief Lenght of the buffer of characters on construction.
     *
     * @note Cannot be zero.
     */
    static const size_t LENGTH_ON_CONSTRUCTION = 7U;
    
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

template <typename T, int32_t L, class R, class A>
BaseString<T,L,R,A>::BaseString() 
    : AbstractBaseString<T,R,A>() {
    bool_t const isConstructed( construct() );
    setConstructed( isConstructed );
}

template <typename T, int32_t L, class R, class A>
BaseString<T,L,R,A>::BaseString(api::String<T> const& source) 
    : AbstractBaseString<T,R,A>() {
    bool_t const isConstructed( construct( source.getChar() ) );
    setConstructed( isConstructed );            
}

template <typename T, int32_t L, class R, class A>
BaseString<T,L,R,A>::BaseString(T const* const source) 
    : AbstractBaseString<T,R,A>() {
    bool_t const isConstructed( construct( source ) );
    setConstructed( isConstructed );            
}

template <typename T, int32_t L, class R, class A>
BaseString<T,L,R,A>::BaseString(int32_t const value, Number::Base const base) 
    : AbstractBaseString<T,R,A>() {    
    bool_t isConstructed( construct() );
    if( isConstructed )
    {
        isConstructed = convert(value, base);
    }
    setConstructed( isConstructed );
}

template <typename T, int32_t L, class R, class A>
BaseString<T,L,R,A>::~BaseString()
{
}

template <typename T, int32_t L, class R, class A>
BaseString<T,L,R,A>::BaseString(BaseString const& obj) ///< SCA MISRA-C++:2008 Justified Rule 12-8-1
    : AbstractBaseString<T,R,A>(obj) {
    bool_t const isConstructed( construct( obj.getChar() ) );
    setConstructed( isConstructed );
}

template <typename T, int32_t L, class R, class A>
BaseString<T,L,R,A>& BaseString<T,L,R,A>::operator=(BaseString const& obj)
{
    if( isConstructed() && (this != &obj) )
    {
        copyRaw3(str_, obj.str_, L);
        Parent::operator=(obj);            
    }
    return *this;
}    

#if EOOS_CPP_STANDARD >= 2011

template <typename T, int32_t L, class R, class A>
BaseString<T,L,R,A>::BaseString(BaseString&& obj) noexcept
    : AbstractBaseString<T,R,A>( move(obj) ) {
    copyRaw3(str_, obj.str_, L);
}

template <typename T, int32_t L, class R, class A>
BaseString<T,L,R,A>& BaseString<T,L,R,A>::operator=(BaseString&& obj) & noexcept
{
    if( isConstructed() && (this != &obj) )
    {
        copyRaw3(str_, obj.str_, L);
        Parent::operator=( move(obj) );            
    }
    return *this;
}        

#endif // EOOS_CPP_STANDARD >= 2011    

template <typename T, int32_t L, class R, class A>
size_t BaseString<T,L,R,A>::getLength() const
{
    size_t length( 0U );
    if( isConstructed() )
    {
        // @todo Rework here not to calculate length, but take it form a class member.
        length = getLengthRaw(str_);
    }
    return length;
}

template <typename T, int32_t L, class R, class A>
T const* BaseString<T,L,R,A>::getChar() const
{
    T const* str( NULLPTR );
    if( isConstructed() )
    {
        str = str_;
    }
    return str;
}

template <typename T, int32_t L, class R, class A>
bool_t BaseString<T,L,R,A>::copyRaw(T const* const str)
{
    bool_t res( false );
    if( isConstructed() && (str != NULLPTR) )
    {
        copyRaw3(str_, str, L);
        res = true;
    }
    return res;
}

template <typename T, int32_t L, class R, class A>
bool_t BaseString<T,L,R,A>::concatenateRaw(T const* const str)
{
    bool_t res( false );
    if( isConstructed() && (str != NULLPTR) )
    {
        concatenateRaw3(str_, str, L);
        res = true;
    }
    return res;

}

template <typename T, int32_t L, class R, class A>
bool_t BaseString<T,L,R,A>::isEqualToRaw(T const* const str) const
{
    bool_t res( false );
    if( isConstructed() && (str != NULLPTR) )
    {
        res = isEqualRaw2(str_, str);
    }
    return res;

}

template <typename T, int32_t L, class R, class A>
bool_t BaseString<T,L,R,A>::construct(T const* const str)
{
    str_[0] = R::getTerminator();
    bool_t res( false );
    if( isConstructed() )
    {
        res = ( str == NULLPTR ) ? true : copyRaw(str);
    }
    return res;
}

template <typename T, class R, class A>
BaseString<T,0,R,A>::BaseString() 
    : AbstractBaseString<T,R,A>()
    , len_(0)
    , str_(NULLPTR){
    bool_t const isConstructed( construct() );
    setConstructed( isConstructed );
}

template <typename T, class R, class A>
BaseString<T,0,R,A>::BaseString(api::String<T> const& source) 
    : AbstractBaseString<T,R,A>()
    , len_(0)
    , str_(NULLPTR){
    bool_t const isConstructed( construct( source.getChar() ) );
    setConstructed( isConstructed );
}

template <typename T, class R, class A>
BaseString<T,0,R,A>::BaseString(T const* const source) 
    : AbstractBaseString<T,R,A>()
    , len_(0)
    , str_(NULLPTR){
    bool_t const isConstructed( construct( source ) );
    setConstructed( isConstructed );
}

template <typename T, class R, class A>
BaseString<T,0,R,A>::BaseString(int32_t const value, Number::Base const base)
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

template <typename T, class R, class A>
BaseString<T,0,R,A>::~BaseString()
{
    free();
}

template <typename T, class R, class A>
BaseString<T,0,R,A>::BaseString(BaseString const& obj) ///< SCA MISRA-C++:2008 Justified Rule 12-8-1
    : AbstractBaseString<T,R,A>(obj)
    , len_(0)
    , str_(NULLPTR){
    bool_t const isConstructed( construct( obj.getChar() ) );
    setConstructed( isConstructed );
}

template <typename T, class R, class A>
BaseString<T,0,R,A>& BaseString<T,0,R,A>::operator=(BaseString const& obj)
{
    if( isConstructed() && (this != &obj) )
    {
        static_cast<void>( copyRaw( obj.getChar() ) );
        Parent::operator=(obj);
    }
    return *this;
}    

#if EOOS_CPP_STANDARD >= 2011

template <typename T, class R, class A>
BaseString<T,0,R,A>::BaseString(BaseString&& obj) noexcept 
    : AbstractBaseString<T,R,A>( move(obj) )
    , len_(obj.len_)
    , str_(obj.str_){
    obj.clean();
}

template <typename T, class R, class A>
BaseString<T,0,R,A>& BaseString<T,0,R,A>::operator=(BaseString&& obj) & noexcept
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

template <typename T, class R, class A>
size_t BaseString<T,0,R,A>::getLength() const
{
    size_t length( 0U );
    if( isConstructed() )
    {
        // @todo Rework here not to calculate length, but take it form a class member.
        length = getLengthRaw(str_);
    }
    return length;
}

template <typename T, class R, class A>
T const* BaseString<T,0,R,A>::getChar() const
{
    T const* str( NULLPTR );
    if( isConstructed() )
    {
        str = str_;
    }
    return str;
}

template <typename T, class R, class A>
bool_t BaseString<T,0,R,A>::copyRaw(T const* const str)
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

template <typename T, class R, class A>
bool_t BaseString<T,0,R,A>::concatenateRaw(T const* const str)
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

template <typename T, class R, class A>
bool_t BaseString<T,0,R,A>::isEqualToRaw(T const* const str) const
{
    bool_t res( false );
    if( isConstructed() && (str != NULLPTR) )
    {
        res = isEqualRaw2(str_, str);
    }
    return res;
}

template <typename T, class R, class A>
bool_t BaseString<T,0,R,A>::construct(T const* const str)
{
    bool_t res( false );
    if( isConstructed() )
    {
        size_t length( LENGTH_ON_CONSTRUCTION );
        if( str != NULLPTR )
        {
            length = getLengthRaw(str);
        }
        bool_t isAllocated( allocate(length) );
        if( isAllocated )
        {
            res = ( str == NULLPTR ) ? true : copyRaw(str);
        }
    }
    return res;
}

template <typename T, class R, class A>
bool_t BaseString<T,0,R,A>::prepareCopy(size_t length)
{
    bool_t res(true);
    if( !isFit(length) )
    {
        free();
        res = allocate(length);
    }
    return res;
}

template <typename T, class R, class A>
bool_t BaseString<T,0,R,A>::prepareConcatenate(size_t length)
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

template <typename T, class R, class A>
bool_t BaseString<T,0,R,A>::allocate(size_t const length)
{
    bool_t res( false );
    if( (str_ == NULLPTR) && (length != 0U) )
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

template <typename T, class R, class A>
void BaseString<T,0,R,A>::free()
{
    if(str_ != NULLPTR)
    {
        A::free(str_);
        str_ = NULLPTR;
        len_ = 0U;
    }
}

template <typename T, class R, class A>
void BaseString<T,0,R,A>::clean()
{
    str_ = NULLPTR;
    len_ = 0U;
}

template <typename T, class R, class A>
size_t BaseString<T,0,R,A>::calculateSize(size_t len)
{
    return (len * sizeof(T)) + sizeof(T);
}

template <typename T, class R, class A>
bool_t BaseString<T,0,R,A>::isFit(size_t len) const ///< SCA MISRA-C++:2008 Justified Rule 2-10-2
{
    return len <= len_;
}

} // namespace lib
} // namespace eoos
#endif // LIB_BASESTRING_HPP_
