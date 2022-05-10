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
    AbstractLinkedList() : Parent(),
        illegal_ (),
        last_    (NULLPTR),
        count_   (0){
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
    AbstractLinkedList(const T& illegal) : Parent(),
        illegal_ (illegal),
        last_    (NULLPTR),
        count_   (0){
    }

    /**
     * @brief Destructor.
     */
    virtual ~AbstractLinkedList()
    {
        clear();
    }

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }

    /**
     * @copydoc eoos::api::List::add(const T&)
     */
    virtual bool_t add(const T& element)
    {
        return Self::isConstructed() ? addNode(getLength(), element) : false;
    }

    /**
     * @copydoc eoos::api::List::add(int32_t,const T&)
     */
    virtual bool_t add(int32_t const index, const T& element)
    {
        return Self::isConstructed() ? addNode(index, element) : false;
    }

    /**
     * @copydoc eoos::api::List::clear()
     */
    virtual void clear()
    {
        if( not Self::isConstructed() )
        {
            return;
        }
        const int32_t b = getLength() - 1;
        for(int32_t i=b; i>=0; i--)
        {
            if( not removeNode( getNodeByIndex(i) ) )
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
    virtual bool_t remove(const int32_t index)
    {
        return Self::isConstructed() ? removeNode( getNodeByIndex(index) ) : false;
    }

    /**
     * @copydoc eoos::api::List::removeElement(const T&)
     */
    virtual bool_t removeElement(const T& element)
    {
        return Self::isConstructed() ? removeNode( getNodeByElement(element) ) : false;
    }

    /**
     * @copydoc eoos::api::Queue::peek(const T&)
     */
    virtual T& peek() const
    {
        return get(0);
    }

    /**
     * @copydoc eoos::api::List::getFirst()
     */
    virtual T& getFirst() const
    {
        return get(0);
    }

    /**
     * @copydoc eoos::api::List::getLast()
     */
    virtual T& getLast() const
    {
        return get( getLength() - 1 );
    }

    /**
     * @copydoc eoos::api::List::get()
     */
    virtual T& get(int32_t index) const
    {
        if( not Self::isConstructed() )
        {
            return illegal_;
        }
        Node* const node = getNodeByIndex(index);
        return (node != NULLPTR) ? node->getElement() : illegal_;
    }

    /**
     * @copydoc eoos::api::Collection::getLength()
     */
    virtual int32_t getLength() const
    {
        return (last_ == NULLPTR) ? 0 : (last_->getIndex() + 1);
    }

    /**
     * @copydoc eoos::api::Collection::isEmpty()
     */
    virtual bool_t isEmpty() const
    {
        return (last_ == NULLPTR) ? true : false;
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
        if( Self::isConstructed() )
        {
            illegal_ = value;
        }
    }

    /**
     * @copydoc eoos::api::IllegalValue::isIllegal(const T&)
     */
    virtual bool_t isIllegal(const T& value) const
    {
        if( not Self::isConstructed() )
        {
            return false;
        }
        return (illegal_ == value) ? true : false;
    }

    /**
     * @copydoc eoos::api::List::getIndexOf(const T&)
     */
    virtual int32_t getIndexOf(const T& element) const
    {
        Node* const node = getNodeByElement(element);
        return (node != NULLPTR) ? node->getIndex() : -1;
    }

    /**
     * @copydoc eoos::api::List::isIndex(const T&)
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
    Buffer<T,0,A>* getAsBuffer() const
    {
        #ifdef EOOS_NO_STRICT_MISRA_RULES
        if( not Self::isConstructed() )
        {
            return NULLPTR;
        }
        const int32_t count = getLength();
        if(count == 0)
        {
            return NULLPTR;
        }
        Buffer<T,0,A>* buf = new Buffer<T,0,A>(count, illegal_);
        if( (buf == NULLPTR) || (not buf->isConstructed()) )
        {
            delete buf;
            return NULLPTR;
        }
        Node* node = last_->getNext();
        for(int32_t i=0; i<count; i++)
        {
            (*buf)[i] = node->getElement();
            node = node->getNext();
        }
        return buf;
        #else
        return NULLPTR;
        #endif
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
    bool_t addNode(const int32_t index, const T& element)
    {
        if(isIndexOutOfBounds(index))
        {
            return false;
        }
        Node* const node = new Node(element);
        if( node == NULLPTR )
        {
            delete node;
            return false;
        }
        if( not node->isConstructed() )
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
            Node* const after = getNodeByIndex(index - 1);
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
            Node* const before = getNodeByIndex(0);
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
    Node* getNodeByIndex(const int32_t index) const
    {
        if( not isIndex(index) )
        {
            return NULLPTR;
        }
        if( index == (getLength() - 1) )
        {
            return last_;
        }
        Node* node = last_->getNext();
        for(int32_t i=0; i<index; i++)
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
    Node* getNodeByElement(const T& element) const
    {
        const int32_t len = getLength();
        if(len == 0)
        {
            return NULLPTR;
        }
        Node* node = last_->getNext();
        for(int32_t i=0; i<len; i++)
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
    bool_t isIndexOutOfBounds(const int32_t index) const
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
        return count_;
    }

    /**
     * @brief Returns reference to self data value.
     *
     * @return Data value.
     */
    Node*& getReferenceToLast()
    {
        return last_;
    }

    /**
     * @brief Returns reference to self data value.
     *
     * @return Data value.
     */
    T& getReferenceToIllegal()
    {
        return illegal_;
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
