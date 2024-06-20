/**
 * @file      lib.AbstractBuffer.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ABSTRACTBUFFER_HPP_
#define LIB_ABSTRACTBUFFER_HPP_

#include "lib.NonCopyable.hpp"
#include "api.SequenceContainer.hpp"
#include "api.IllegalValue.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class AbstractBuffer<T,A>
 * @brief Abstract buffer class.
 *
 * @tparam T Data type of buffer element.
 * @tparam A Heap memory allocator class.
 *
 * @todo Inherit Object instead of NonCopyable. 
 */
template <typename T, class A = Allocator>
class AbstractBuffer : public NonCopyable<A>, public api::SequenceContainer<T>, public api::IllegalValue<T>
{
    typedef NonCopyable<A> Parent;

public:

    using api::SequenceContainer<T>::getData;

    /**
     * @brief Destructor.
     */
    virtual ~AbstractBuffer();

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const;

    /**
     * @copydoc eoos::api::Collection::getLength()
     */
    virtual size_t getLength() const;

    /**
     * @copydoc eoos::api::Collection::isEmpty()
     */
    virtual bool_t isEmpty() const;

    /**
     * @copydoc eoos::api::IllegalValue::getIllegal()
     */
    virtual T const& getIllegal() const;

    /**
     * @copydoc eoos::api::IllegalValue::setIllegal(T const&)
     */
    virtual void setIllegal(T const& value);

    /**
     * @copydoc eoos::api::IllegalValue::isIllegal()
     */
    virtual bool_t isIllegal(T const& value) const;

    /**
     * @brief Fills this buffer by given value.
     *
     * @param value A filling value.
     */
    void fill(T const& value);

    /**
     * @brief Fills this buffer by given value.
     *
     * @param value  Filling value.
     * @param length Count of filling elements.
     */
    void fill(T const& value, size_t const length);

    /**
     * @brief Fills this buffer by given value.
     *
     * @param value Filling value.
     * @param index Begin index.
     * @param count Count of filling elements.
     */
    void fill(T const& value, size_t const begin, size_t const count);

    /**
     * @brief Returns an element of this buffer.
     *
     * @param index An element index.
     * @return An element.
     */
    T& operator[](size_t const index);

protected:

    /**
     * @brief Constructor.
     *
     * @param length Count of buffer elements.
     */
    explicit AbstractBuffer(size_t length);

    /**
     * @brief Constructor.
     *
     * @note A passed illegal element will be copied to an internal data of the class
     *
     * @param length  Count of buffer elements.
     * @param illegal Illegal value.
     */
    AbstractBuffer(size_t length, T const& illegal);

    /**
     * @brief Copies buffer to buffer.
     *
     * If the source buffer greater than this buffer,
     * then only cropped data of that will be and copied.
     *
     * @param buf Reference to source buffer.
     */
    void copy(api::SequenceContainer<T> const& buf);

private:

    /**
     * @brief Number of elements of this buffer.
     */
    size_t length_;

    /**
     * @brief Illegal element of this buffer.
     */
    T illegal_;

};

template <typename T, class A>
AbstractBuffer<T,A>::~AbstractBuffer()
{
}

template <typename T, class A>
bool_t AbstractBuffer<T,A>::isConstructed() const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
{
    return Parent::isConstructed();
}

template <typename T, class A>
size_t AbstractBuffer<T,A>::getLength() const
{
    size_t length( 0U );
    if( isConstructed() )
    {
        length = length_;
    }
    return length;
}

template <typename T, class A>
bool_t AbstractBuffer<T,A>::isEmpty() const
{
    bool_t res( true );
    if( isConstructed() )
    {
        res = length_ == 0U;
    }
    return res;
}

template <typename T, class A>
T const& AbstractBuffer<T,A>::getIllegal() const
{
    return illegal_;
}

template <typename T, class A>
void AbstractBuffer<T,A>::setIllegal(T const& value)
{
    illegal_ = value;
}

template <typename T, class A>
bool_t AbstractBuffer<T,A>::isIllegal(T const& value) const
{
    return illegal_ == value;
}

template <typename T, class A>
void AbstractBuffer<T,A>::fill(T const& value)
{
    fill(value, length_);
}

template <typename T, class A>
void AbstractBuffer<T,A>::fill(T const& value, size_t const length)
{
    fill(value, 0, length);
}

template <typename T, class A>
void AbstractBuffer<T,A>::fill(T const& value, size_t const begin, size_t const count)
{
    bool_t const hasIndex( begin < length_ );
    if( isConstructed() && hasIndex )
    {
        T* const buf( getData() );
        size_t const length( begin + count );
        // MSVC warning C4003 of Most Vexing Parse case avoided with no nameing the variable 'max'
        size_t const maximum( ( length <= length_ ) ? length : length_ );
        for(size_t i(begin); i<maximum; i++)
        {
            buf[i] = value;
        }
    }
}

template <typename T, class A>
T& AbstractBuffer<T,A>::operator[](size_t const index)
{
    T* value;
    T* const buf( getData() );
    if( (!isConstructed()) || (index >= length_) || (buf == NULLPTR) )
    {
        value = &illegal_;
    }
    else
    {
        value = &buf[index];
    }
    return *value;
}

template <typename T, class A>
AbstractBuffer<T,A>::AbstractBuffer(size_t length) 
    : NonCopyable<A>()
    , api::SequenceContainer<T>()
    , api::IllegalValue<T>()
    , length_(length)
    , illegal_(){
}

template <typename T, class A>
AbstractBuffer<T,A>::AbstractBuffer(size_t length, T const& illegal) 
    : NonCopyable<A>()
    , api::SequenceContainer<T>()
    , api::IllegalValue<T>()
    , length_(length)
    , illegal_(illegal){
}

template <typename T, class A>
void AbstractBuffer<T,A>::copy(api::SequenceContainer<T> const& buf)
{
    if( isConstructed() )
    {
        size_t const size1( getLength() );
        size_t const size2( buf.getLength() );
        size_t const size( ( size1 < size2 ) ? size1 : size2 );
        T* const buf1( getData() );
        T* const buf2( buf.getData() );
        for(size_t i(0U); i<size; i++)
        {
            buf1[i] = buf2[i];
        }
    }
}

} // namespace lib
} // namespace eoos
#endif // LIB_ABSTRACTBUFFER_HPP_
