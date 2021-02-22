/**
 * @brief Abstract class for some buffers.
 *
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
 * @brief Primary template implementation.
 *
 * @tparam T - data type of buffer element.
 * @tparam A - heap memory allocator class.
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
     * @param length - count of buffer elements.
     */
    explicit AbstractBuffer(int32_t length) : Parent(),
        length_  (length),
        illegal_ (){
    }

    /**
     * @brief Constructor.
     *
     * NOTE: A passed illegal element will be copied to an internal data of the class
     *
     * @param length - count of buffer elements.
     * @param illegal - illegal value.
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
     * @brief Tests if this object has been constructed.
     *
     * @return true if object has been constructed successfully.
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }

    /**
     * @brief Returns a number of elements.
     *
     * @return number of elements.
     */
    virtual int32_t getLength() const
    {
        return length_;
    }

    /**
     * @brief Tests if this collection has elements.
     *
     * @return true if this collection does not contain any elements.
     */
    virtual bool_t isEmpty() const
    {
        return length_ == 0;
    }

    /**
     * @brief Returns illegal element which will be returned as error value.
     *
     * If illegal value is not set method returns uninitialized variable.
     *
     * @return reference to illegal element.
     */
    virtual T& getIllegal() const
    {
        return illegal_;
    }

    /**
     * @brief Sets illegal element which will be returned as error value.
     *
     * @param value - an illegal value.
     */
    virtual void setIllegal(const T& value)
    {
        illegal_ = value;
    }

    /**
     * @brief Tests if given value is an illegal.
     *
     * @param value - testing value.
     * @param true if value is an illegal.
     */
    virtual bool_t isIllegal(const T& value) const
    {
        return illegal_ == value;
    }

    /**
     * @brief Fills this buffer by given value.
     *
     * @param value - a filling value.
     */
    void fill(const T& value)
    {
        fill(value, length_);
    }

    /**
     * @brief Fills this buffer by given value.
     *
     * @param value - filling value.
     * @param length - count of filling elements.
     */
    void fill(const T& value, const int32_t length)
    {
        fill(value, 0, length);
    }

    /**
     * @brief Fills this buffer by given value.
     *
     * @param value - filling value.
     * @param index - begin index.
     * @param count - count of filling elements.
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
     * @param index - an element index.
     * @return an element.
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
     * @return pointer to buffer or NULLPTR.
     */
    virtual T* getBuffer() const = 0;

    /**
     * @brief Copies buffer to buffer.
     *
     * If the source buffer greater than this buffer,
     * then only cropped data of that will be and copied.
     *
     * @param buf - reference to source buffer.
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
     * @brief Copy constructor.
     *
     * @param obj - reference to source object.
     */
    AbstractBuffer(const AbstractBuffer& obj);

    /**
     * @brief Assignment operator.
     *
     * @param obj - reference to source object.
     * @return reference to this object.
     */
    AbstractBuffer& operator=(const AbstractBuffer& obj);

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
