/**
 * @file      lib.AbstractList.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ABSTRACTLIST_HPP_
#define LIB_ABSTRACTLIST_HPP_

#include "lib.NonCopyable.hpp"
#include "lib.LinkedNode.hpp"
#include "lib.UniquePointer.hpp"
#include "api.List.hpp"
#include "api.Queue.hpp"
#include "api.Iterable.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class AbstractList<T,A>
 * @brief Abstract list class.
 *
 * @tparam T Data type of container element.
 * @tparam A Heap memory allocator class.
 */
template <typename T, class A = Allocator>
class AbstractList :
    public NonCopyable<A>,
    public api::List<T>,
    public api::Queue<T>,
    public api::Iterable<T>{

    typedef NonCopyable<A> Parent;
    typedef LinkedNode<T,A> Node;

public:
        
    using api::List<T>::getListIterator;

    /**
     * @brief Destructor.
     */
    virtual ~AbstractList();

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const;

    /**
     * @copydoc eoos::api::List::add(T const&)
     *
     * @todo This function implements the pure virtual functions of api::List<T> and api::Queue<T> interfaces,
     *       that might be incorrect, thus this has to be deeply analyzed.
     */
    virtual bool_t add(T const& element);

    /**
     * @copydoc eoos::api::List::add(int32_t,T const&)
     */
    virtual bool_t add(int32_t const index, T const& element);

    /**
     * @copydoc eoos::api::List::clear()
     */
    virtual void clear();

    /**
     * @copydoc eoos::api::List::removeFirst()
     */
    virtual bool_t removeFirst();

    /**
     * @copydoc eoos::api::List::removeLast()
     */
    virtual bool_t removeLast();

    /**
     * @copydoc eoos::api::Queue::remove()
     */
    virtual bool_t remove();

    /**
     * @copydoc eoos::api::List::remove(int32_t)
     */
    virtual bool_t remove(int32_t const index);

    /**
     * @copydoc eoos::api::List::removeElement(T const&)
     */
    virtual bool_t removeElement(T const& element);

    /**
     * @copydoc eoos::api::Queue::peek(T const&)
     */
    virtual T& peek();

    /**
     * @copydoc eoos::api::List::getFirst()
     */
    virtual T& getFirst();

    /**
     * @copydoc eoos::api::List::getLast()
     */
    virtual T& getLast();

    /**
     * @copydoc eoos::api::List::get()
     */
    virtual T& get(int32_t index);

    /**
     * @copydoc eoos::api::Collection::getLength()
     *
     * @todo Re-implement to return value of a member variable;
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
     * @copydoc eoos::api::List::getIndexOf(T const&)
     */
    virtual int32_t getIndexOf(T const& element) const;

    /**
     * @copydoc eoos::api::List::isIndex(T const&)
     */
    virtual bool_t isIndex(int32_t const index) const;
    
    /**
     * @copydoc eoos::api::Iterable::getIterator()
     */
    virtual api::Iterator<T>* getIterator();

protected:

    /**
     * @brief Constructor.
     */
    AbstractList();

    /**
     * @brief Constructor.
     *
     * @note A passed element must be copied to an internal data structure of
     * this class by calling a copy constructor so that the element
     * might be invalidated after the function called.
     *
     * @param illegal An illegal element.
     */
    AbstractList(T const& illegal);

    /**
     * @brief Inserts new element to the specified position in this list.
     *
     * Given element will be copied to self nodes structure by a copy constructor calling.
     *
     * @param index   Position in this list.
     * @param element Inserting element.
     * @return True if element is inserted.
     */
    bool_t addNode(int32_t const index, T const& element);

    /**
     * @brief Returns a node of this list by index.
     *
     * @param index Position in this list.
     * @return Pointer to the node of this list.
     */
    Node* getNodeByIndex(int32_t const index);

    /**
     * @brief Returns a node of this list by element.
     *
     * @param element Reference to element.
     * @return Pointer to the node of this list.
     */
    Node* getNodeByElement(T const& element) const;

    /**
     * @brief Removes a node of this list.
     *
     * @param node Pointer to node.
     * @return True if a node is removed successfully.
     */
    bool_t removeNode(Node* const node);

    /**
     * @brief Tests if index is out of this list bounds.
     *
     * @param index Checking position in this list.
     * @return True if index is outed.
     */
    bool_t isIndexOutOfBounds(int32_t const index) const;

    /**
     * @brief Returns reference to self data value.
     *
     * @return Data value.
     */
    uint32_t& getReferenceToCount();

    /**
     * @brief Returns reference to self data value.
     *
     * @return Data value.
     */
    Node*& getReferenceToLast();

    /**
     * @brief Returns reference to self data value.
     *
     * @return Data value.
     */
    T& getReferenceToIllegal();

private:

    /**
     * @brief Illegal element of this list.
     */
    T illegal_;

    /**
     * @brief Last node of this list.
     */
    Node* last_;

    /**
     * @brief Number of changes in this list.
     */
    uint32_t count_;

};

template <typename T, class A>
AbstractList<T,A>::~AbstractList()
{
    AbstractList<T,A>::clear();
}

template <typename T, class A>
bool_t AbstractList<T,A>::isConstructed() const ///< SCA MISRA-C++:2008 Justified Rule 10-3-1 and Defected Rule 9-3-3
{
    return Parent::isConstructed();
}

template <typename T, class A>
bool_t AbstractList<T,A>::add(T const& element)
{
    size_t const lenght( getLength() );
    return isConstructed() ? addNode(static_cast<int32_t>(lenght), element) : false;
}

template <typename T, class A>
bool_t AbstractList<T,A>::add(int32_t const index, T const& element)
{
    return isConstructed() ? addNode(index, element) : false;
}

template <typename T, class A>
void AbstractList<T,A>::clear()
{
    if( isConstructed() )
    {
        size_t lenght( getLength() );
        while(true)
        {
            if(lenght == 0U)
            {
                break;
            }
            Node* const node( getNodeByIndex(0) );
            static_cast<void>( removeNode(node) );
            lenght -= 1U;
        }
    }
}

template <typename T, class A>
bool_t AbstractList<T,A>::removeFirst()
{
    return remove(0);
}

template <typename T, class A>
bool_t AbstractList<T,A>::removeLast()
{
    size_t const length( getLength() );
    return remove( static_cast<int32_t>(length) - 1 );
}

template <typename T, class A>
bool_t AbstractList<T,A>::remove()
{
    return remove(0);
}

template <typename T, class A>
bool_t AbstractList<T,A>::remove(int32_t const index)
{
    return isConstructed() ? removeNode( getNodeByIndex(index) ) : false;
}

template <typename T, class A>
bool_t AbstractList<T,A>::removeElement(T const& element)
{
    return isConstructed() ? removeNode( getNodeByElement(element) ) : false;
}

template <typename T, class A>
T& AbstractList<T,A>::peek()
{
    return get(0);
}

template <typename T, class A>
T& AbstractList<T,A>::getFirst()
{
    return get(0);
}

template <typename T, class A>
T& AbstractList<T,A>::getLast()
{
    size_t const length( getLength() );
    return get( static_cast<int32_t>(length) - 1 );
}

template <typename T, class A>
T& AbstractList<T,A>::get(int32_t index)
{   
    T* element( &illegal_ );
    if( isConstructed() )
    {
        Node* const node( getNodeByIndex(index) );
        if(node != NULLPTR)
        {
            element = &node->getElement();
        }
    }
    return *element;
}

template <typename T, class A>
size_t AbstractList<T,A>::getLength() const
{
    size_t length( 0U );
    if( isConstructed() )
    {
        length = (last_ == NULLPTR) ? 0U : ( static_cast<size_t>(last_->getIndex()) + 1U);
    }
    return length;
}

template <typename T, class A>
bool_t AbstractList<T,A>::isEmpty() const
{
    bool_t res( true );
    if( isConstructed() )
    {
        res =(last_ == NULLPTR) ? true : false;
    }
    return res;
}

template <typename T, class A>
T const& AbstractList<T,A>::getIllegal() const
{
    return illegal_;
}

template <typename T, class A>
void AbstractList<T,A>::setIllegal(T const& value)
{
    if( isConstructed() )
    {
        illegal_ = value;
    }
}

template <typename T, class A>
bool_t AbstractList<T,A>::isIllegal(T const& value) const
{
    bool_t res( false );
    if( isConstructed() )
    {
        res = illegal_ == value;
    }
    return res;
}

template <typename T, class A>
int32_t AbstractList<T,A>::getIndexOf(T const& element) const
{
    Node* const node( getNodeByElement(element) );
    return (node != NULLPTR) ? node->getIndex() : api::List<T>::ERROR_INDEX;
}

template <typename T, class A>
bool_t AbstractList<T,A>::isIndex(int32_t const index) const
{
    bool_t res( false );
    if( 0 <= index )
    {
        if( static_cast<size_t>(index) < getLength() )
        {
            res = true;
        }
    }
    return res;
}

template <typename T, class A>
api::Iterator<T>* AbstractList<T,A>::getIterator() ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
{
    return getListIterator(0);
}

template <typename T, class A>
AbstractList<T,A>::AbstractList() 
    : NonCopyable<A>()
    , api::List<T>()
    , api::Queue<T>()
    , api::Iterable<T>()
    , illegal_()
    , last_(NULLPTR)
    , count_(0) {
}

template <typename T, class A>
AbstractList<T,A>::AbstractList(T const& illegal) 
    : NonCopyable<A>()
    , api::List<T>()
    , api::Queue<T>()
    , api::Iterable<T>()
    , illegal_(illegal)
    , last_(NULLPTR)
    , count_(0) {
}

template <typename T, class A>
bool_t AbstractList<T,A>::addNode(int32_t const index, T const& element)
{
    bool_t res( false );
    if( !isIndexOutOfBounds(index) )
    {
        UniquePointer<Node,SmartPointerDeleter<Node>,A> node( new Node(element) );
        if( !node.isNull() && node->isConstructed() )
        {
            if(last_ == NULLPTR)
            {
                last_ = node.get();
                res = true;
            }
            else
            {
                if(index > 0)
                {
                    Node* const after( getNodeByIndex(index - 1) );
                    if(after != NULLPTR)
                    {
                        after->insertAfter(node.get());
                        if(after == last_)
                        {
                            last_ = node.get();
                        }
                        res = true;
                    }
                }
                else
                {
                    Node* const before( getNodeByIndex(0) );
                    if(before != NULLPTR)
                    {                        
                        before->insertBefore(node.get());
                        res = true;
                    }
                }
            }
        }
        if( res == true )
        {
            static_cast<void>( node.release() );
            count_++;
        }
    }
    return res;
}

template <typename T, class A>
typename AbstractList<T,A>::Node* AbstractList<T,A>::getNodeByIndex(int32_t const index)
{
    Node* node( NULLPTR );
    if( isIndex(index) )
    {
        size_t lenght( getLength() );
        if( static_cast<size_t>(index) == (lenght - 1U) )
        {
            node = last_;
        }
        else
        {
            node = last_->getNext();
            for(int32_t i(0); i<index; i++)
            {
                node = node->getNext();
            }
        }
    }
    return node;
}

template <typename T, class A>
typename AbstractList<T,A>::Node* AbstractList<T,A>::getNodeByElement(T const& element) const
{
    Node* res( NULLPTR );
    size_t const len( getLength() );
    if(len != 0U)
    {
        Node* node( last_->getNext() );
        for(size_t i(0U); i<len; i++)
        {
            if(element != node->getElement())
            {
                node = node->getNext();
                continue;
            }
            res = node;
            break;
        } ///< UT Justified Line: Compiler dependency
    }
    return res;
}

template <typename T, class A>
bool_t AbstractList<T,A>::removeNode(Node* const node)
{
    bool_t res( false );
    if(node != NULLPTR)
    {
        if(node == last_)
        {
            if(getLength() == 1U)
            {
                last_ = NULLPTR;
            }
            else
            {
                last_ = last_->getPrevious();
            }
        }
        delete node;
        count_++;
        res = true;
    }
    return res;
}

template <typename T, class A>
bool_t AbstractList<T,A>::isIndexOutOfBounds(int32_t const index) const
{
    bool_t res( false ); 
    if( index < 0 )
    {
        res = true;
    }
    else if( static_cast<size_t>(index) > getLength() )
    {
        res = true;
    }
    else
    {
        res = false;
    }
    return res;
}

template <typename T, class A>
uint32_t& AbstractList<T,A>::getReferenceToCount()
{
    return count_; ///< SCA MISRA-C++:2008 Justified Rule 9-3-2
}

template <typename T, class A>
typename AbstractList<T,A>::Node*& AbstractList<T,A>::getReferenceToLast()
{
    return last_; ///< SCA MISRA-C++:2008 Justified Rule 9-3-2
}

template <typename T, class A>
T& AbstractList<T,A>::getReferenceToIllegal()
{
    return illegal_; ///< SCA MISRA-C++:2008 Justified Rule 9-3-2
}
        
} // namespace lib
} // namespace eoos
#endif // LIB_ABSTRACTLIST_HPP_
