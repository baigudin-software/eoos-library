/**
 * @file      lib.Fifo.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_FIFO_HPP_
#define LIB_FIFO_HPP_

#include "lib.NonCopyable.hpp"
#include "api.Queue.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class Fifo<T,L,A>
 * @brief Abstract list class.
 *
 * @tparam T Data type of container element.
 * @tparam L Maximum number of FIFO elements. 
 * @tparam A Heap memory allocator class.
 */
template <typename T, int32_t L, class A = Allocator>
class Fifo : public NonCopyable<A>, public api::Queue<T> {

    typedef NonCopyable<A> Parent;

public:

    /**
     * @brief Constructor.
     *
     * [Default mode]
     * FIFO is not locked on overrun. Once the FIFO is full the next incoming
     * message will overwrite the previous one.          
     */
    Fifo();

    /**
     * @brief Constructor.
     *
     * [Default mode]
     * FIFO is not locked on overrun. Once the FIFO is full the next incoming
     * message will overwrite the previous one.
     *
     * FIFO is locked against overrun. Once the FIFO is full the next incoming
     * message will be discarded.     
     *
     * @param isLocked FIFO locked mode flag.
     */    
    Fifo(bool_t isLocked);
    
    /**
     * @brief Constructor.
     *
     * [Default mode]
     * FIFO is not locked on overrun. Once the FIFO is full the next incoming
     * message will overwrite the previous one.
     *
     * FIFO is locked against overrun. Once the FIFO is full the next incoming
     * message will be discarded.     
     *
     * @param isLocked FIFO locked mode flag.
     * @param illegal An illegal value.     
     */    
    Fifo(bool_t isLocked, T const& illegal);

    /**
     * @brief Destructor.
     */
    virtual ~Fifo();

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const;

    /**
     * @copydoc eoos::api::Queue::add(T const&)
     */
    virtual bool_t add(T const& element);

    /**
     * @copydoc eoos::api::Queue::remove()
     */
    virtual bool_t remove();

    /**
     * @copydoc eoos::api::Queue::peek(T const&)
     */
    virtual T& peek();

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
     * @copydoc eoos::api::IllegalValue::isIllegal(T const&)
     */
    virtual bool_t isIllegal(T const& value) const;    
    
    /**
     * @brief Tests if this FIFO is full.
     *
     * @return True if this FIFO is full.
     */
    bool_t isFull() const;
    
    /**
     * @brief Tests if this FIFO is overrun.
     *
     * @return True if this FIFO is overrun.
     */
    bool_t isOverrun() const;

    /**
     * @brief Tests if this FIFO is locked.
     *
     * @return True if this FIFO is locked.
     */
    bool_t isLocked() const;

protected:

    using Parent::setConstructed;    

private:
    
    /**
     * @brief Constructs this object.
     *
     * @return true if object has been constructed successfully.
     */
    bool_t construct();

    /**
     * @brief FIFO elements.
     */
    T elements_[L];

    /**
     * @brief Illegal value.
     */    
    T illegal_;
        
    /**
     * @brief FIFO locked mode.
     */    
    bool_t isLocked_;

    /**
     * @brief Head element index.
     */    
    int32_t head_;
    
    /**
     * @brief Tail element index is fist free.
     */    
    int32_t tail_;

    /**
     * @brief FIFO message pending.
     *
     * Indicate how many messages are pending in the FIFO.
     */    
    int32_t length_;

    /**
     * @brief FIFO overrun.
     *
     * Set when a new message has been added while the FIFO was full.
     */    
    bool_t isOverrun_;

    /**
     * @brief FIFO full.
     * 
     * Set when L messages are stored in the FIFO.
     */    
    bool_t isFull_;
};

template <typename T, int32_t L, class A>
Fifo<T,L,A>::Fifo()
    : lib::NonCopyable<A>()
    , api::Queue<T>()
    , illegal_()
    , isLocked_( false )
    , head_( 0 )
    , tail_( 0 )
    , length_( 0 )
    , isOverrun_( false )
    , isFull_( false ){
    bool_t const isConstructed( construct() );
    setConstructed( isConstructed );        
}

template <typename T, int32_t L, class A>
Fifo<T,L,A>::Fifo(bool_t isLocked)
    : lib::NonCopyable<A>()
    , api::Queue<T>()
    , illegal_()
    , isLocked_( isLocked )
    , head_( 0 )
    , tail_( 0 )
    , length_( 0 )
    , isOverrun_( false )
    , isFull_( false ){
    bool_t const isConstructed( construct() );
    setConstructed( isConstructed );        
}

template <typename T, int32_t L, class A>
Fifo<T,L,A>::Fifo(bool_t isLocked, T const& illegal)
    : lib::NonCopyable<A>()
    , api::Queue<T>()
    , illegal_( illegal )
    , isLocked_( isLocked )
    , head_( 0 )
    , tail_( 0 )
    , length_( 0 )
    , isOverrun_( false )
    , isFull_( false ){
    bool_t const isConstructed( construct() );
    setConstructed( isConstructed );        
}

template <typename T, int32_t L, class A>
Fifo<T,L,A>::~Fifo()
{
}

template <typename T, int32_t L, class A>
bool_t Fifo<T,L,A>::isConstructed() const
{
    return Parent::isConstructed();
}

template <typename T, int32_t L, class A>
bool_t Fifo<T,L,A>::add(T const& element)
{
    bool_t res( false );
    if( isConstructed() )
    {
        if( !isFull_ )
        {
            elements_[tail_] = element;
            tail_ += 1;
            if( tail_ == L )
            {
                tail_ = 0;
            }
            if( tail_ == head_ )
            {
                isFull_ = true; 
            }
            length_ += 1;
            res = true;
        }
        else
        {
            if( !isLocked_ )
            {
                int32_t last = tail_ - 1;
                if( last < 0 )
                {
                    last = L - 1;
                }
                elements_[last] = element;
                res = true;                
            }
            isOverrun_ = true;
        }
    }
    return res;
}

template <typename T, int32_t L, class A>
bool_t Fifo<T,L,A>::remove()
{
    bool_t res( false );
    if( isConstructed() )
    {
        if( length_ != 0 )
        {
            head_ += 1;
            if( head_ == L )
            {
                head_ = 0;
            }
            isFull_ = false;
            isOverrun_ = false;            
            length_ -= 1;
            res = true;
        }
    }
    return res;
}

template <typename T, int32_t L, class A>
T& Fifo<T,L,A>::peek()
{
    T* element = &illegal_;
    if( isConstructed() ) 
    {
        if( length_ != 0 )
        {
            element = &elements_[head_];
        }
    }
    return *element;
}

template <typename T, int32_t L, class A>
size_t Fifo<T,L,A>::getLength() const
{
    return static_cast<size_t>(length_);
}

template <typename T, int32_t L, class A>
bool_t Fifo<T,L,A>::isEmpty() const
{
    return (length_ == 0);
}

template <typename T, int32_t L, class A>
bool_t Fifo<T,L,A>::isFull() const
{
    return isFull_;
}
    
template <typename T, int32_t L, class A>
bool_t Fifo<T,L,A>::isOverrun() const
{
    return isOverrun_;
}

template <typename T, int32_t L, class A>
bool_t Fifo<T,L,A>::isLocked() const
{
    return isLocked_;
}

template <typename T, int32_t L, class A>
T const& Fifo<T,L,A>::getIllegal() const
{
    return illegal_;
}

template <typename T, int32_t L, class A>
void Fifo<T,L,A>::setIllegal(T const& value)
{
    if( isConstructed() )
    {
        illegal_ = value;
    }
}

template <typename T, int32_t L, class A>
bool_t Fifo<T,L,A>::isIllegal(T const& value) const
{
    bool_t res( false );
    if( isConstructed() )
    {
        res = illegal_ == value;
    }
    return res;
}

template <typename T, int32_t L, class A>
bool_t Fifo<T,L,A>::construct()
{
    bool_t res( false );
    if( isConstructed() )
    {
        res = true;
    }
    return res;    
}

} // namespace lib
} // namespace eoos
#endif // LIB_FIFO_HPP_
