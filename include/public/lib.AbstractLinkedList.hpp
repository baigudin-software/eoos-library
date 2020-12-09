/**
 * @brief Abstract class for sequential accessing to data store.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ABSTRACT_LINKED_LIST_HPP_
#define LIB_ABSTRACT_LINKED_LIST_HPP_

#include "lib.Object.hpp"
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
 * @brief Primary template implementation.
 *
 * @param T data type of container element.
 * @param A heap memory allocator class.
 */
template <typename T, class A = Allocator>
class AbstractLinkedList :
    public Object<A>,
    public api::List<T>,
    public api::Queue<T>,
    public api::Iterable<T>{

    typedef AbstractLinkedList<T,A> Self;
    typedef ::eoos::lib::Object<A> Parent;
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
     * NOTE: A passed element must be copied to an internal data structure of
     * this class by calling a copy constructor so that the element
     * might be invalidated after the function called.
     *
     * @param illegal - an illegal element.
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
     * @brief Tests if this object has been constructed.
     *
     * @return true if object has been constructed successfully.
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }

    /**
     * @brief Inserts new element to the end of this list.
     *
     * @param element inserting element.
     * @return true if element is added.
     */
    virtual bool_t add(const T& element)
    {
        return Self::isConstructed() ? addNode(getLength(), element) : false;
    }

    /**
     * @brief Inserts new element to the specified position in this list.
     *
     * @param index   position in this list.
     * @param element inserting element.
     * @return true if element is inserted.
     */
    virtual bool_t add(int32_t const index, const T& element)
    {
        return Self::isConstructed() ? addNode(index, element) : false;
    }

    /**
     * @brief Removes all elements from this list.
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
     * @brief Removes the first element from this list.
     *
     * @return true if an element is removed successfully.
     */
    virtual bool_t removeFirst()
    {
        return remove(0);
    }

    /**
     * @brief Removes the last element from this list.
     *
     * @return true if an element is removed successfully.
     */
    virtual bool_t removeLast()
    {
        return remove( getLength() - 1 );
    }

    /**
     * @brief Removes the head element of this queue or list.
     *
     * @return true if an element is removed successfully.
     */
    virtual bool_t remove()
    {
        return remove(0);
    }

    /**
     * @brief Removes the element at the specified position in this list.
     *
     * @param index   position in this list.
     * @return true if an element is removed successfully.
     */
    virtual bool_t remove(const int32_t index)
    {
        return Self::isConstructed() ? removeNode( getNodeByIndex(index) ) : false;
    }

    /**
     * @brief Removes the first occurrence of the specified element from this list.
     *
     * @param element reference to element.
     * @return true if an element is removed successfully.
     */
    virtual bool_t removeElement(const T& element)
    {
        return Self::isConstructed() ? removeNode( getNodeByElement(element) ) : false;
    }

    /**
     * @brief Examines the head element of this container.
     *
     * @return the head element.
     */
    virtual T& peek() const
    {
        return get(0);
    }

    /**
     * @brief Returns the first element in this container.
     *
     * @return the first element in this container.
     */
    virtual T& getFirst() const
    {
        return get(0);
    }

    /**
     * @brief Returns the last element in this container.
     *
     * @return the last element in this container.
     */
    virtual T& getLast() const
    {
        return get( getLength() - 1 );
    }

    /**
     * @brief Returns an element from this container by index.
     *
     * @param index - position in this container.
     * @return indexed element of this container.
     */
    virtual T& get(int32_t index) const
    {
        if( not Self::isConstructed() )
        {
            return illegal_;
        }
        Node* const node = getNodeByIndex(index);
        return node != NULLPTR ? node->getElement() : illegal_;
    }

    /**
     * @brief Returns a number of elements in this list.
     *
     * @return number of elements.
     */
    virtual int32_t getLength() const
    {
        return last_ == NULLPTR ? 0 : last_->getIndex() + 1;
    }

    /**
     * @brief Tests if this list has elements.
     *
     * @return true if this list does not contain any elements.
     */
    virtual bool_t isEmpty() const
    {
        return last_ == NULLPTR ? true : false;
    }

    /**
     * @brief Returns illegal element which will be returned as error value.
     *
     * If illegal value is not set method returns uninitialized variable.
     *
     * @return illegal element.
     */
    virtual T& getIllegal() const
    {
        return illegal_;
    }

    /**
     * @brief Sets illegal element which will be returned as error value.
     *
     * @param value illegal value.
     */
    virtual void setIllegal(const T& value)
    {
        if( Self::isConstructed() )
        {
            illegal_ = value;
        }
    }

    /**
     * @brief Tests if given value is an illegal.
     *
     * @param value testing value.
     * @param true if value is an illegal.
     */
    virtual bool_t isIllegal(const T& value) const
    {
        if( not Self::isConstructed() )
        {
            return false;
        }
        return illegal_ == value ? true : false;
    }

    /**
     * @brief Returns the index of the first occurrence of the specified element in this list.
     *
     * @param element reference to the element.
     * @return index or -1 if this list does not contain the element.
     */
    virtual int32_t getIndexOf(const T& element) const
    {
        Node* const node = getNodeByElement(element);
        return node != NULLPTR ? node->getIndex() : -1;
    }

    /**
     * @brief Tests if given index is available.
     *
     * @param index checking position in this list.
     * @return true if index is present.
     */
    virtual bool_t isIndex(int32_t const index) const
    {
        return (0 <= index && index < getLength()) ? true : false;
    }

    /**
     * @brief Returns an array of all list links to elements.
     *
     * You have to call delete operator for returned value after it have used.
     *
     * @return pointer to reference of elements or NULLPTR if list is empty.
     */
    virtual Buffer<T,0,A>* array() const
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
        if(buf == NULLPTR || not buf->isConstructed())
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

    /**
     * @brief Returns an iterator of this list elements.
     *
     * @return pointer to new itererator.
     */
    virtual api::Iterator<T>* getIterator()
    {
        return this->getListIterator(0);
    }

protected:

    /**
     * @brief Inserts new element to the specified position in this list.
     *
     * Given element will be copied to self nodes structure by a copy constructor calling.
     *
     * @param index   position in this list.
     * @param element inserting element.
     * @return true if element is inserted.
     */
    bool_t addNode(const int32_t index, const T& element)
    {
        if(isIndexOutOfBounds(index))
        {
            return false;
        }
        Node* const node = new Node(element);
        if(node == NULLPTR || not node->isConstructed())
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
     * @param index position in this list.
     * @return pointer to the node of this list.
     */
    Node* getNodeByIndex(const int32_t index) const
    {
        if( not isIndex(index) )
        {
            return NULLPTR;
        }
        if(index == getLength() - 1)
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
     * @param element reference to element.
     * @return pointer to the node of this list.
     */
    Node* getNodeByElement(const T& element) const
    {
        const int32_t len = getLength();
        if(len == 0)
        {
            return NULLPTR;
        }
        Node* node = last_->getNext();
        for(int32_t i=0; i<len; i++, node = node->getNext())
        {
            if(element != node->getElement())
            {
                continue;
            }
            return node;
        }
        return NULLPTR;
    }

    /**
     * @brief Removes a node of this list.
     *
     * @param node pointer to node.
     * @return true if a node is removed successfully.
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
     * @param index checking position in this list.
     * @return true if index is outed.
     */
    bool_t isIndexOutOfBounds(const int32_t index) const
    {
        return (index < 0 || index > getLength()) ? true : false;
    }

    /**
     * @brief Returns reference to self data value.
     *
     * @return data value.
     */
    int32_t& getReferenceToCount()
    {
        return count_;
    }

    /**
     * @brief Returns reference to self data value.
     *
     * @return data value.
     */
    Node*& getReferenceToLast()
    {
        return last_;
    }

    /**
     * @brief Returns reference to self data value.
     *
     * @return data value.
     */
    T& getReferenceToIllegal()
    {
        return illegal_;
    }

private:

    /**
     * @brief Copy constructor.
     *
     * @param obj reference to source object.
     */
    AbstractLinkedList(const AbstractLinkedList& obj);

    /**
     * @brief Assignment operator.
     *
     * @param obj reference to source object.
     * @return reference to this object.
     */
    AbstractLinkedList& operator=(const AbstractLinkedList& obj);

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
#endif // LIB_ABSTRACT_LINKED_LIST_HPP_
