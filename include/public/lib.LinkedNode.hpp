/**
 * @brief Element for linked lists.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_LINKED_NODE_HPP_
#define LIB_LINKED_NODE_HPP_

#include "lib.Object.hpp"

namespace eoos
{
namespace lib
{

/**
 * @brief Primary template implementation.
 *
 * @tparam T data type of element.
 * @tparam A heap memory allocator class.
 */
template <typename T, class A = Allocator>
class LinkedNode : public Object<A>
{
    typedef LinkedNode<T,A> Self;
    typedef ::eoos::lib::Object<A> Parent;

public:

    /**
     * @brief Constructor.
     *
     * NOTE: A passed element will be copied to the internal data
     * structure by calling a copy constructor so that the element
     * might be invalidated after the function called.
     *
     * @param element an user element of this node.
     */
    LinkedNode(const T& element) : Parent(),
        prev_    (this),
        next_    (this),
        index_   (0),
        element_ (element){
    }

    /**
     * @brief Destructor.
     */
    virtual ~LinkedNode()
    {
        LinkedNode* node = this->next_;
        while(node->index_ != 0)
        {
            node->index_--;
            node = node->next_;
        }
        next_->prev_ = prev_;
        prev_->next_ = next_;
        prev_ = this;
        next_ = this;
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
     * @brief Inserts a new element after this.
     *
     * Method links a node after this and reindexes
     * chain of nodes starts from given node.
     *
     * @param node pointer to inserted node.
     */
    virtual void insertAfter(LinkedNode<T,A>* node)
    {
        link(node);
        node->index_ = index_;
        do
        {
            node->index_++;
            node = node->next_;
        }
        while(node->index_ != 0);
    }

    /**
     * @brief Inserts a new element after this.
     *
     * Method links a node before this and reindexes
     * chain of nodes starts from this node.
     *
     * @param node pointer to inserted node.
     */
    virtual void insertBefore(LinkedNode<T,A>* node)
    {
        prev_->link(node);
        node->index_ = index_;
        node = this;
        do
        {
            node->index_++;
            node = node->next_;
        }
        while(node->index_ != 0);
    }

    /**
     * @brief Returns previous element.
     *
     * @return previous element.
     */
    virtual LinkedNode<T,A>* getPrevious() const
    {
        return prev_;
    }

    /**
     * @brief Returns next element.
     *
     * @return next element.
     */
    virtual LinkedNode<T,A>* getNext() const
    {
        return next_;
    }

    /**
     * @brief Returns the element.
     *
     * @return next element.
     */
    virtual T& getElement() const
    {
        return element_;
    }

    /**
     * @brief Returns the element index.
     *
     * @return element index.
     */
    virtual int32_t getIndex() const
    {
        return index_;
    }

private:

    /**
     * @brief Links a given node after this.
     *
     * @param node pointer to linking node.
     */
    void link(LinkedNode<T,A>* node)
    {
        next_->prev_ = node;
        node->next_ = next_;
        next_ = node;
        node->prev_ = this;
    }

    /**
     * @brief Copy constructor.
     *
     * @param obj reference to source object.
     */
    LinkedNode(const LinkedNode& obj);

    /**
     * @brief Assignment operator.
     *
     * @param obj reference to source object.
     * @return reference to this object.
     */
    LinkedNode& operator=(const LinkedNode& obj);

    /**
     * @brief Previous node.
     */
    LinkedNode* prev_;

    /**
     * @brief Next node.
     */
    LinkedNode* next_;

    /**
     * @brief Index of the node.
     */
    int32_t index_;

    /**
     * @brief Containing element.
     */
    mutable T element_;

};

} // namespace lib
} // namespace eoos
#endif // LIB_LINKED_NODE_HPP_
