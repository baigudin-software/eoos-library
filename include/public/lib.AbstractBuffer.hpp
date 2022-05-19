/**
 * @file      lib.AbstractBuffer.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ABSTRACTBUFFER_HPP_
#define LIB_ABSTRACTBUFFER_HPP_

#include "lib.NonCopyable.hpp"
#include "api.Collection.hpp"
#include "api.IllegalValue.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class AbstractBuffer<T,A>
 * @brief Abstract class for some buffers.
 *
 * @tparam T Data type of buffer element.
 * @tparam A Heap memory allocator class.
 */
template <typename T, class A = Allocator>
class AbstractBuffer : public NonCopyable<A>, public api::Collection<T>, public api::IllegalValue<T>
{
    typedef AbstractBuffer<T,A>  Self;
    typedef NonCopyable<A> Parent;

public:

    /**
     * @brief Constructor.
     *
     * @param length Count of buffer elements.
     */
    explicit AbstractBuffer(int32_t length) 
        : NonCopyable<A>()
        , api::Collection<T>()
        , api::IllegalValue<T>()
        , length_(length)
        , illegal_(){
    }

    /**
     * @brief Constructor.
     *
     * @note A passed illegal element will be copied to an internal data of the class
     *
     * @param length  Count of buffer elements.
     * @param illegal Illegal value.
     */
    AbstractBuffer(int32_t length, T const& illegal) 
        : NonCopyable<A>()
        , api::Collection<T>()
        , api::IllegalValue<T>()
        , length_(length)
        , illegal_(illegal){
    }

    /**
     * @brief Destructor.
     */
    virtual ~AbstractBuffer()
    {
    }

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        return Parent::isConstructed();
    }

    /**
     * @copydoc eoos::api::Collection::getLength()
     */
    virtual int32_t getLength() const
    {
        return length_;
    }

    /**
     * @copydoc eoos::api::Collection::isEmpty()
     */
    virtual bool_t isEmpty() const
    {
        return length_ == 0;
    }

    /**
     * @copydoc eoos::api::IllegalValue::getIllegal()
     */
    virtual T const& getIllegal() const
    {
        return illegal_;
    }

    /**
     * @copydoc eoos::api::IllegalValue::setIllegal(T const&)
     */
    virtual void setIllegal(T const& value)
    {
        illegal_ = value;
    }

    /**
     * @copydoc eoos::api::IllegalValue::isIllegal()
     */
    virtual bool_t isIllegal(T const& value) const
    {
        return illegal_ == value;
    }

    /**
     * @brief Fills this buffer by given value.
     *
     * @param value A filling value.
     */
    void fill(T const& value)
    {
        fill(value, length_);
    }

    /**
     * @brief Fills this buffer by given value.
     *
     * @param value  Filling value.
     * @param length Count of filling elements.
     */
    void fill(T const& value, int32_t const length)
    {
        fill(value, 0, length);
    }

    /**
     * @brief Fills this buffer by given value.
     *
     * @param value Filling value.
     * @param index Begin index.
     * @param count Count of filling elements.
     */
    void fill(T const& value, int32_t const index, int32_t const count)
    {
        bool_t const hasIndex( index < length_ );
        if( Self::isConstructed() && hasIndex )
        {
            T* const buf( getBuffer() );
            int32_t const length( index + count );
            // MSVC warning C4003 of Most Vexing Parse case avoided with no nameing the variable 'max'
            int32_t const maximum( ( length <= length_ ) ? length : length_ );
            for(int32_t i(index); i<maximum; i++)
            {
                buf[i] = value;
            }
        }
    }

    /**
     * @brief Returns an element of this buffer.
     *
     * @param index An element index.
     * @return An element.
     */
    T& operator[](int32_t const index)
    {
        T* value;
        T* const buf( getBuffer() );
        if( (!Self::isConstructed()) || (index >= length_) || (buf == NULLPTR) )
        {
            value = &illegal_;
        }
        else
        {
            value = &buf[index];
        }
        return *value;
    }

protected:

    /**
     * @brief Returns a pointer to the fist buffer element.
     *
     * @return Pointer to buffer or NULLPTR.
     */
    virtual T* getBuffer() const = 0;

    /**
     * @brief Copies buffer to buffer.
     *
     * If the source buffer greater than this buffer,
     * then only cropped data of that will be and copied.
     *
     * @param buf Reference to source buffer.
     */
    void copy(AbstractBuffer const& buf)
    {
        if( Self::isConstructed() )
        {
            int32_t const size1( getLength() );
            int32_t const size2( buf.getLength() );
            int32_t const size( ( size1 < size2 ) ? size1 : size2 );
            T* const buf1( getBuffer() );
            T* const buf2( buf.getBuffer() );
            for(int32_t i(0); i<size; i++)
            {
                buf1[i] = buf2[i];
            }
        }
    }

private:

    /**
     * @brief Number of elements of this buffer.
     */
    int32_t length_;

    /**
     * @brief Illegal element of this buffer.
     */
    mutable T illegal_;

};

} // namespace lib
} // namespace eoos
#endif // LIB_ABSTRACTBUFFER_HPP_
