/**
 * @file      lib.Buffer.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_BUFFER_HPP_
#define LIB_BUFFER_HPP_

#include "lib.AbstractBuffer.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class Buffer<T,L,A>
 * @brief Buffer class static.
 *
 * This class is a primary template defines a realization that contains
 * a whole buffer, which is defined by a template argument.
 *
 * @tparam T Data type of buffer element.
 * @tparam L Maximum number of buffer elements, or 0 for dynamic allocation.
 * @tparam A Heap memory allocator class.
 */
template <typename T, int32_t L, class A = Allocator>
class Buffer : public AbstractBuffer<T,A>
{
    typedef AbstractBuffer<T,A> Parent;

public:

    /**
     * @brief Constructor.
     */
    Buffer() : Parent(L),
        buf_ (arr_){
    }

    /**
     * @brief Constructor.
     *
     * @note A passed illegal element will be copied to an internal data of the class
     *
     * @param illegal An illegal value.
     */
    Buffer(const T& illegal) : Parent(L, illegal),
        buf_ (arr_){
    }

    /**
     * @brief Destructor.
     */
    virtual ~Buffer()
    {
    }

    /**
     * @brief Assignment operator.
     *
     * If the source buffer is greater than this buffer,
     * only cropped data of that will be copied.
     *
     * @param buf Reference to source buffer.
     * @return Reference to this object.
     */
    Buffer& operator=(const Buffer<T,L,A>& buf)
    {
        this->copy(buf);
        return *this;
    }

    /**
     * @brief Assignment operator.
     *
     * If the source buffer is greater than this buffer,
     * only cropped data of that will be copied.
     *
     * @param buf Reference to source buffer.
     * @return Reference to this object.
     */
    Buffer& operator=(const AbstractBuffer<T,A>& buf)
    {
        this->copy(buf);
        return *this;
    }

protected:

    /**
     * @brief Returns a pointer to the fist buffer element.
     *
     * @return Pointer to buffer, or NULLPTR.
     */
    virtual T* getBuffer() const
    {
        return buf_;
    }

private:    

    /**
     * @brief Current array of T elements.
     */
    T arr_[L];

    /**
     * @brief Pointer to current array.
     *
     * @note The variable has been defined only for giving the getBuffer member function to be constant.
     */
    T* buf_;

};

#ifdef EOOS_NO_STRICT_MISRA_RULES

/**
 * @class Buffer<T,A>
 * @brief Buffer class dynamic.
 *
 * This is a partial specialization of the template allocates necessary 
 * memory size for containing the buffer in a heap memory.
 *
 * @tparam T Data type of buffer element.
 * @tparam A Heap memory allocator class.
 */
template <typename T, class A>
class Buffer<T,0,A> : public AbstractBuffer<T,A>
{
    typedef AbstractBuffer<T,A> ParentSpec1;

public:

    /**
     * @brief Constructor.
     *
     * @param length Count of buffer elements.
     */
    explicit Buffer(int32_t const length) : ParentSpec1(length),
        buf_       (NULLPTR),
        isDeleted_ (true){
        const bool_t isConstructed = construct(length);
        this->setConstructed( isConstructed );
    }

    /**
     * @brief Constructor.
     *
     * @note A passed illegal element will be copied to an internal data of the class
     *
     * @param length  Count of buffer elements.
     * @param illegal Illegal value.
     */
    Buffer(int32_t const length, const T& illegal) : ParentSpec1(length, illegal),
        buf_       (NULLPTR),
        isDeleted_ (true){
        const bool_t isConstructed = construct(length);
        this->setConstructed( isConstructed );
    }

    /**
     * @brief Constructor.
     *
     * @note Given external buffer has to exist until this object is alive.
     *
     * @param length Number of elements.
     * @param buf    Pointer to external buffer.
     */
    Buffer(int32_t const length, T*  const buf) : ParentSpec1(length),
        buf_       (buf),
        isDeleted_ (false){
        const bool_t isConstructed = construct(length);
        this->setConstructed( isConstructed );
    }

    /**
     * @brief Constructor.
     *
     * NOTE 1: Given external buffer has to exist until this object is alive.
     * NOTE 2: A passed illegal element will be copied to an internal data of the class.
     *
     * @param length  Number of elements.
     * @param buf     Pointer to external buffer.
     * @param illegal Illegal value.
     */
    Buffer(int32_t const length, T* const buf, const T& illegal) : ParentSpec1(length, illegal),
        buf_       (buf),
        isDeleted_ (false){
        const bool_t isConstructed = construct(length);
        this->setConstructed( isConstructed );
    }


    /**
     * @brief Destructor.
     */
    virtual ~Buffer()
    {
        if( isDeleted_ == true )
        {
            A::free(buf_);
        }
    }

    /**
     * @brief Assignment operator.
     *
     * If the source buffer is greater than this buffer,
     * only cropped data of that will be copied.
     *
     * @param buf Reference to source buffer.
     * @return Reference to this object.
     */
    Buffer& operator=(const Buffer<T,0,A>& buf)
    {
        this->copy(buf);
        return *this;
    }

    /**
     * @brief Assignment operator.
     *
     * If the source buffer is greater than this buffer,
     * only cropped data of that will be copied.
     *
     * @param buf Reference to source buffer.
     * @return Reference to this object.
     */
    Buffer& operator=(const AbstractBuffer<T,A>& buf)
    {
        this->copy(buf);
        return *this;
    }

protected:

    /**
     * @brief Returns a pointer to the fist buffer element.
     *
     * @return Pointer to buffer, or NULLPTR.
     */
    virtual T* getBuffer() const
    {
        T* buf;
        if( not ParentSpec1::isConstructed() )
        {
            buf = NULLPTR;
        }
        else
        {
            buf = buf_;
        }
        return buf;
    }

private:

    /**
     * @brief Constructor.
     *
     * @param length Count of buffer elements.
     * @return Boolean result.
     */
    bool_t construct(size_t const length)
    {
        bool_t res;
        if( ParentSpec1::isConstructed() )
        {
            if(buf_ == NULLPTR)
            {
                void* const addr = A::allocate(length * (sizeof(T)));
                buf_ = reinterpret_cast<T*>( addr );
            }
            res = buf_ != NULLPTR;
        }
        else
        {
            res = false;
        }
        return res;
    }

    /**
     * @brief Pointer to external given or self created array.
     */
    T* buf_;

    /**
     * @brief Deletet flag.
     *
     * Is set to true for self created array or
     * set to false for external given array.
     */
    bool_t isDeleted_;

};

#endif // EOOS_NO_STRICT_MISRA_RULES

} // namespace lib
} // namespace eoos
#endif // LIB_BUFFER_HPP_
