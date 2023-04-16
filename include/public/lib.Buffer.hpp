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

    using Parent::isConstructed;

    /**
     * @brief Constructor.
     */
    Buffer() 
        : AbstractBuffer<T,A>(L)
        , buf_(arr_){
    }

    /**
     * @brief Constructor.
     *
     * @note A passed illegal element will be copied to an internal data of the class
     *
     * @param illegal An illegal value.
     */
    Buffer(T const& illegal) 
        : AbstractBuffer<T,A>(L, illegal)
        , buf_ (arr_){
    }

    /**
     * @brief Destructor.
     */
    virtual ~Buffer()
    {
    }
    
    /**
     * @copydoc eoos::api::SequenceContainer::getData()
     */
    virtual T* getData() const
    {
        T* buf( NULLPTR );
        if( isConstructed() )
        {
            buf = buf_;
        }
        return buf;
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
    Buffer& operator=(api::SequenceContainer<T> const& buf)
    {
        if( isConstructed() && buf.isConstructed() )
        {        
            copy(buf);
        }
        return *this;
    }
    
protected:

    using Parent::setConstructed;
    using Parent::copy;

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

#ifndef EOOS_GLOBAL_ENABLE_NO_HEAP

/**
 * @class Buffer<T,0,A>
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
    typedef AbstractBuffer<T,A> Parent;

public:

    using Parent::isConstructed;

    /**
     * @brief Constructor.
     *
     * @param length Count of buffer elements.
     */
    explicit Buffer(size_t const length) 
        : AbstractBuffer<T,A>(length)
        , buf_(NULLPTR)
        , isDeleted_(true) {
        bool_t const isConstructed( construct(length) );
        setConstructed( isConstructed );
    }

    /**
     * @brief Constructor.
     *
     * @note A passed illegal element will be copied to an internal data of the class
     *
     * @param length  Count of buffer elements.
     * @param illegal Illegal value.
     */
    Buffer(size_t const length, T const& illegal) 
        : AbstractBuffer<T,A>(length, illegal)
        , buf_(NULLPTR)
        , isDeleted_(true) {
        bool_t const isConstructed( construct(length) );
        setConstructed( isConstructed );
    }

    /**
     * @brief Constructor.
     *
     * @note Given external buffer has to exist until this object is alive.
     *
     * @param length Number of elements.
     * @param buf    Pointer to external buffer.
     */
    Buffer(size_t const length, T* const buf) 
        : AbstractBuffer<T,A>(length)
        , buf_(buf)
        , isDeleted_(false) {
        bool_t const isConstructed( construct(length) );
        setConstructed( isConstructed );
    }

    /**
     * @brief Constructor.
     *
     * @note Given external buffer has to exist until this object is alive.
     * @note A passed illegal element will be copied to an internal data of the class.
     *
     * @param length  Number of elements.
     * @param buf     Pointer to external buffer.
     * @param illegal Illegal value.
     */
    Buffer(size_t const length, T* const buf, T const& illegal) 
        : AbstractBuffer<T,A>(length, illegal)
        , buf_(buf)
        , isDeleted_(false) {
        bool_t const isConstructed( construct(length) );
        setConstructed( isConstructed );
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
     * @copydoc eoos::api::SequenceContainer::getData()
     */
    virtual T* getData() const
    {
        T* buf( NULLPTR );
        if( isConstructed() )
        {
            buf = buf_;
        }
        return buf;
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
    Buffer& operator=(api::SequenceContainer<T> const& buf)
    {
        if( isConstructed() && buf.isConstructed() )
        {        
            copy(buf);
        }
        return *this;
    }
    
protected:

    using Parent::setConstructed;    
    using Parent::copy;

private:

    /**
     * @brief Constructor.
     *
     * @param length Count of buffer elements.
     * @return Boolean result.
     */
    bool_t construct(size_t const length)
    {
        bool_t res( false );
        if( isConstructed() && (length > 0U) )
        {
            if(buf_ == NULLPTR)
            {
                void* const addr( A::allocate(length * (sizeof(T))) );
                buf_ = reinterpret_cast<T*>( addr ); ///< SCA MISRA-C++:2008 Justified Rule 5-2-8
            }
            res = buf_ != NULLPTR;
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

#endif // EOOS_GLOBAL_ENABLE_NO_HEAP

} // namespace lib
} // namespace eoos
#endif // LIB_BUFFER_HPP_
