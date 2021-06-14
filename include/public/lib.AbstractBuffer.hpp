/**
 * @file      lib.AbstractBuffer.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ABSTRACT_BUFFER_HPP_
#define LIB_ABSTRACT_BUFFER_HPP_

#include "lib.Object.hpp"
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
class AbstractBuffer : public Object<A>, public api::Collection<T>, public api::IllegalValue<T>
{
    typedef AbstractBuffer<T,A>  Self;
    typedef ::eoos::lib::Object<A> Parent;

public:

    /**
     * @brief Constructor.
     *
     * @param length Count of buffer elements.
     */
    explicit AbstractBuffer(int32_t length) : Parent(),
        length_  (length),
        illegal_ (){
    }

    /**
     * @brief Constructor.
     *
     * @note A passed illegal element will be copied to an internal data of the class
     *
     * @param length  Count of buffer elements.
     * @param illegal Illegal value.
     */
    AbstractBuffer(int32_t length, const T& illegal) : Parent(),
        length_  (length),
        illegal_ (illegal){
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
    virtual bool_t isConstructed() const
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
    virtual T& getIllegal() const
    {
        return illegal_;
    }

    /**
     * @copydoc eoos::api::IllegalValue::setIllegal(const T&)
     */
    virtual void setIllegal(const T& value)
    {
        illegal_ = value;
    }

    /**
     * @copydoc eoos::api::IllegalValue::isIllegal()
     */
    virtual bool_t isIllegal(const T& value) const
    {
        return illegal_ == value;
    }

    /**
     * @brief Fills this buffer by given value.
     *
     * @param value A filling value.
     */
    void fill(const T& value)
    {
        fill(value, length_);
    }

    /**
     * @brief Fills this buffer by given value.
     *
     * @param value  Filling value.
     * @param length Count of filling elements.
     */
    void fill(const T& value, const int32_t length)
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
    void fill(const T& value, const int32_t index, const int32_t count)
    {
        const bool_t hasIndex = index < length_;
        if( Self::isConstructed() && hasIndex )
        {
            T* const buf = getBuffer();
            const int32_t length = index + count;
            const int32_t max = ( length <= length_ ) ? length : length_;
            for(int32_t i=index; i<max; i++)
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
        T* const buf = getBuffer();
        if( not Self::isConstructed() || (index >= length_) || (buf == NULLPTR) )
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
    void copy(const AbstractBuffer& buf)
    {
        if( Self::isConstructed() )
        {
            const int32_t size1 = getLength();
            const int32_t size2 = buf.getLength();
            const int32_t size = ( size1 < size2 ) ? size1 : size2;
            T* const buf1 = getBuffer();
            T* const buf2 = buf.getBuffer();
            for(int32_t i=0; i<size; i++)
            {
                buf1[i] = buf2[i];
            }
        }
    }

private:

    /**
     * @copydoc eoos::Object::Object(const Object&)
     */
    AbstractBuffer(const AbstractBuffer& obj);

    /**
     * @copydoc eoos::Object::operator=(const Object&)
     */
    AbstractBuffer& operator=(const AbstractBuffer& obj);
    
    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(const Object&&)
     */       
    AbstractBuffer(AbstractBuffer&& obj) noexcept = delete; 
    
    /**
     * @copydoc eoos::Object::operator=(const Object&&)
     */
    AbstractBuffer& operator=(AbstractBuffer&& obj) noexcept = delete;
    
    #endif // EOOS_CPP_STANDARD >= 2011    

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
#endif // LIB_ABSTRACT_BUFFER_HPP_
