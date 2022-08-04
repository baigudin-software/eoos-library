/**
 * @file      lib.AbstractLinkedList.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ABSTRACTLINKEDLIST_HPP_
#define LIB_ABSTRACTLINKEDLIST_HPP_

#include "lib.NonCopyable.hpp"
#include "lib.Buffer.hpp"
#include "lib.LinkedNode.hpp"
#include "api.List.hpp"
#include "api.Queue.hpp"
#include "api.Iterable.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class AbstractLinkedList<T,A>
 * @brief Abstract class for sequential accessing to data store.
 *
 * @tparam T Data type of container element.
 * @tparam A Heap memory allocator class.
 */
template <typename T, class A = Allocator>
class AbstractLinkedList :
    public NonCopyable<A>,
    public api::List<T>,
    public api::Queue<T>,
    public api::Iterable<T>{

    typedef AbstractLinkedList<T,A> Self;
    typedef NonCopyable<A> Parent;
    typedef LinkedNode<T,A> Node;

public:

    /**
     * @brief Constructor.
     */
    AbstractLinkedList() 
        : NonCopyable<A>()
        , api::List<T>()
        , api::Queue<T>()
        , api::Iterable<T>()
        , illegal_()
        , last_(NULLPTR)
        , count_(0) {
    }

    /**
     * @brief Constructor.
     *
     * @note A passed element must be copied to an internal data structure of
     * this class by calling a copy constructor so that the element
     * might be invalidated after the function called.
     *
     * @param illegal An illegal element.
     */
    AbstractLinkedList(T const& illegal) 
        : NonCopyable<A>()
        , api::List<T>()
        , api::Queue<T>()
        , api::Iterable<T>()
        , illegal_(illegal)
        , last_(NULLPTR)
        , count_(0) {
    }

    /**
     * @brief Destructor.
     */
    virtual ~AbstractLinkedList()
    {
        Self::clear();
    }

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const ///< SCA MISRA-C++:2008 Justified Rule 10-3-1 and Defected Rule 9-3-3
    {
        return Parent::isConstructed();
    }

    /**
     * @copydoc eoos::api::List::add(T const&)
     */
    virtual bool_t add(T const& element)
    {
        return Self::isConstructed() ? addNode(getLength(), element) : false;
    }

    /**
     * @copydoc eoos::api::List::add(int32_t,T const&)
     */
    virtual bool_t add(int32_t const index, T const& element)
    {
        return Self::isConstructed() ? addNode(index, element) : false;
    }

    /**
     * @copydoc eoos::api::List::clear()
     */
    virtual void clear()
    {
        if( !Self::isConstructed() )
        {
            return;
        }
        size_t const b( getLength() - 1 );
        for(size_t i(b); i>=0; i--)
        {
            if( !removeNode( getNodeByIndex(i) ) )
            {
                break;
            }
        }
    }

    /**
     * @copydoc eoos::api::List::removeFirst()
     */
    virtual bool_t removeFirst()
    {
        return remove(0);
    }

    /**
     * @copydoc eoos::api::List::removeLast()
     */
    virtual bool_t removeLast()
    {
        return remove( getLength() - 1 );
    }

    /**
     * @copydoc eoos::api::Queue::remove()
     */
    virtual bool_t remove()
    {
        return remove(0);
    }

    /**
     * @copydoc eoos::api::List::remove(int32_t)
     */
    virtual bool_t remove(int32_t const index)
    {
        return Self::isConstructed() ? removeNode( getNodeByIndex(index) ) : false;
    }

    /**
     * @copydoc eoos::api::List::removeElement(T const&)
     */
    virtual bool_t removeElement(T const& element)
    {
        return Self::isConstructed() ? removeNode( getNodeByElement(element) ) : false;
    }

    /**
     * @copydoc eoos::api::Queue::peek(T const&)
     */
    virtual T& peek()
    {
        return get(0);
    }

    /**
     * @copydoc eoos::api::List::getFirst()
     */
    virtual T& getFirst()
    {
        return get(0);
    }

    /**
     * @copydoc eoos::api::List::getLast()
     */
    virtual T& getLast()
    {
        return get( getLength() - 1 );
    }

    /**
     * @copydoc eoos::api::List::get()
     */
    virtual T& get(int32_t index)
    {
        if( !Self::isConstructed() )
        {
            return illegal_; ///< SCA MISRA-C++:2008 Justified Rule 9-3-2
        }
        Node* const node( getNodeByIndex(index) );
        return (node != NULLPTR) ? node->getElement() : illegal_;
    }

    /**
     * @copydoc eoos::api::Collection::getLength()
     */
    virtual size_t getLength() const
    {
        size_t length( 0 );
        if( isConstructed() )
        {
            length = (last_ == NULLPTR) ? 0 : (last_->getIndex() + 1);
        }
        return length;
    }

    /**
     * @copydoc eoos::api::Collection::isEmpty()
     */
    virtual bool_t isEmpty() const
    {
        bool_t res( true );
        if( isConstructed() )
        {
            res =(last_ == NULLPTR) ? true : false;
        }
        return res;
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
        if( Self::isConstructed() )
        {
            illegal_ = value;
        }
    }

    /**
     * @copydoc eoos::api::IllegalValue::isIllegal(T const&)
     */
    virtual bool_t isIllegal(T const& value) const
    {
        if( !Self::isConstructed() )
        {
            return false;
        }
        return (illegal_ == value) ? true : false;
    }

    /**
     * @copydoc eoos::api::List::getIndexOf(T const&)
     */
    virtual int32_t getIndexOf(T const& element) const
    {
        Node* const node( getNodeByElement(element) );
        return (node != NULLPTR) ? node->getIndex() : -1;
    }

    /**
     * @copydoc eoos::api::List::isIndex(T const&)
     */
    virtual bool_t isIndex(int32_t const index) const
    {
        if( 0 <= index )
        {
            if( index < getLength() )
            {
                return true;
            }
        }
        return false;
    }
    

    /**
     * @copydoc eoos::api::Iterable::getIterator()
     */
    virtual api::Iterator<T>* getIterator()
    {
        return this->getListIterator(0);
    }    

    /**
     * @brief Returns an array of all list links to elements.
     *
     * You have to call delete operator for returned value after it have used.
     *
     * @return Pointer to reference of elements or NULLPTR if list is empty.
     */
    Buffer<T,0,A>* getAsBuffer()
    {
        #ifdef EOOS_ENABLE_DYNAMIC_HEAP_MEMORY
        if( !Self::isConstructed() )
        {
            return NULLPTR;
        }
        size_t const count( getLength() );
        if(count == 0)
        {
            return NULLPTR;
        }
        Buffer<T,0,A>* buf( new Buffer<T,0,A>(count, illegal_) );
        if( (buf == NULLPTR) || (!buf->isConstructed()) )
        {
            delete buf;
            return NULLPTR;
        }
        Node* node( last_->getNext() );
        for(int32_t i(0); i<count; i++)
        {
            (*buf)[i] = node->getElement();
            node = node->getNext();
        }
        return buf;
        #else
        return NULLPTR;
        #endif // EOOS_ENABLE_DYNAMIC_HEAP_MEMORY
    }

protected:

    /**
     * @brief Inserts new element to the specified position in this list.
     *
     * Given element will be copied to self nodes structure by a copy constructor calling.
     *
     * @param index   Position in this list.
     * @param element Inserting element.
     * @return True if element is inserted.
     *
     * @todo Shall re-implemented according MISRA-C++:2008 Rule 6–6–5 as the delete operator is used. 
     */
    bool_t addNode(int32_t const index, T const& element)
    {
        if(isIndexOutOfBounds(index))
        {
            return false;
        }
        Node* const node( new Node(element) );
        if( node == NULLPTR )
        {
            delete node;
            return false;
        }
        if( !node->isConstructed() )
        {
            delete node;
            return false;
        }        
        if(last_ == NULLPTR)
        {
            last_ = node;
            count_++;
            return true;
        }
        if(index > 0)
        {
            Node* const after( getNodeByIndex(index - 1) );
            if(after == NULLPTR)
            {
                delete node;
                return false;
            }
            after->insertAfter(node);
            if(after == last_)
            {
                last_ = node;
            }
        }
        else
        {
            Node* const before( getNodeByIndex(0) );
            if(before == NULLPTR)
            {
                delete node;
                return false;
            }
            before->insertBefore(node);
        }
        count_++;
        return true;
    }

    /**
     * @brief Returns a node of this list by index.
     *
     * @param index Position in this list.
     * @return Pointer to the node of this list.
     */
    Node* getNodeByIndex(int32_t const index)
    {
        if( !isIndex(index) )
        {
            return NULLPTR;
        }
        if( index == (getLength() - 1) )
        {
            return last_;
        }
        Node* node( last_->getNext() );
        for(int32_t i(0); i<index; i++)
        {
            node = node->getNext();
        }
        return node;
    }

    /**
     * @brief Returns a node of this list by element.
     *
     * @param element Reference to element.
     * @return Pointer to the node of this list.
     */
    Node* getNodeByElement(T const& element) const
    {
        size_t const len( getLength() );
        if(len == 0)
        {
            return NULLPTR;
        }
        Node* node( last_->getNext() );
        for(int32_t i(0); i<len; i++)
        {
            if(element != node->getElement())
            {
                node = node->getNext();
                continue;
            }
            return node;
        }
        return NULLPTR;
    }

    /**
     * @brief Removes a node of this list.
     *
     * @param node Pointer to node.
     * @return True if a node is removed successfully.
     */
    bool_t removeNode(Node* const node)
    {
        if(node == NULLPTR)
        {
            return false;
        }
        if(node == last_)
        {
            if(getLength() == 1)
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
        return true;
    }

    /**
     * @brief Tests if index is out of this list bounds.
     *
     * @param index Checking position in this list.
     * @return True if index is outed.
     */
    bool_t isIndexOutOfBounds(int32_t const index) const
    {
        if( index < 0 )
        {
            return true;
        }
        else if( index > getLength() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
     * @brief Returns reference to self data value.
     *
     * @return Data value.
     */
    int32_t& getReferenceToCount()
    {
        return count_; ///< SCA MISRA-C++:2008 Justified Rule 9-3-2
    }

    /**
     * @brief Returns reference to self data value.
     *
     * @return Data value.
     */
    Node*& getReferenceToLast()
    {
        return last_; ///< SCA MISRA-C++:2008 Justified Rule 9-3-2
    }

    /**
     * @brief Returns reference to self data value.
     *
     * @return Data value.
     */
    T& getReferenceToIllegal()
    {
        return illegal_; ///< SCA MISRA-C++:2008 Justified Rule 9-3-2
    }

private:

    /**
     * @brief Illegal element of this list.
     */
    mutable T illegal_;

    /**
     * @brief Last node of this list.
     */
    Node* last_;

    /**
     * @brief Number of changes in this list.
     */
    int32_t count_;

};
        
} // namespace lib
} // namespace eoos
#endif // LIB_ABSTRACTLINKEDLIST_HPP_
