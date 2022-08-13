/**
 * @file      lib.LinkedNode.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_LINKEDNODE_HPP_
#define LIB_LINKEDNODE_HPP_

#include "lib.NonCopyable.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class LinkedNode<T,A>
 * @brief Element for linked lists.
 *
 * @tparam T Data type of element.
 * @tparam A Heap memory allocator class.
 */
template <typename T, class A = Allocator>
class LinkedNode : public NonCopyable<A>
{
    typedef NonCopyable<A> Parent;

public:

    /**
     * @brief Constructor.
     *
     * @note A passed element will be copied to the internal data
     * structure by calling a copy constructor so that the element
     * might be invalidated after the function called.
     *
     * @param element An user element of this node.
     */
    LinkedNode(T const& element) 
        : NonCopyable<A>()
        , prev_(this)
        , next_(this)
        , index_(0)
        , element_(element) {
    }

    /**
     * @brief Destructor.
     */
    virtual ~LinkedNode()
    {
        LinkedNode* node( this->next_ );
        while(node->index_ != 0)
        {
            node->index_--; ///< SCA MISRA-C++:2008 Defected Rule 5-2-10
            node = node->next_;
        }
        next_->prev_ = prev_;
        prev_->next_ = next_;
        prev_ = this;
        next_ = this;
    }

    /**
     * @brief Inserts a new element after this.
     *
     * Function links a node after this and reindexes
     * chain of nodes starts from given node.
     *
     * @param node Pointer to inserted node.
     */
    void insertAfter(LinkedNode<T,A>* node)
    {
        link(node);
        node->index_ = index_;
        do
        {
            node->index_++; ///< SCA MISRA-C++:2008 Defected Rule 5-2-10
            node = node->next_;
        }
        while(node->index_ != 0);
    }

    /**
     * @brief Inserts a new element before this.
     *
     * Function links a node before this and reindexes
     * chain of nodes starts from this node.
     *
     * @param node Pointer to inserted node.
     */
    void insertBefore(LinkedNode<T,A>* node)
    {
        prev_->link(node);
        node->index_ = index_;
        node = this;
        do
        {
            node->index_++; ///< SCA MISRA-C++:2008 Defected Rule 5-2-10
            node = node->next_;
        }
        while(node->index_ != 0);
    }

    /**
     * @brief Returns previous element.
     *
     * @return Previous element.
     */
    LinkedNode<T,A>* getPrevious() ///< SCA MISRA-C++:2008 Justified Rule 9-3-3
    {
        return prev_;
    }

    /**
     * @brief Returns previous element.
     *
     * @return Previous element.
     */
    LinkedNode<T,A> const* getPrevious() const
    {
        return prev_;
    }

    /**
     * @brief Returns next element.
     *
     * @return The next element.
     */
    LinkedNode<T,A>* getNext() ///< SCA MISRA-C++:2008 Justified Rule 9-3-3
    {
        return next_;
    }

    /**
     * @brief Returns next element.
     *
     * @return The next element.
     */
    LinkedNode<T,A> const* getNext() const
    {
        return next_;
    }

    /**
     * @brief Returns the element.
     *
     * @return The next element.
     */
    T& getElement()
    {
        return element_; ///< SCA MISRA-C++:2008 Justified Rule 9-3-2
    }

    /**
     * @brief Returns the element.
     *
     * @return The next element.
     */
    T const& getElement() const
    {
        return element_;
    }

    /**
     * @brief Returns the element index.
     *
     * @return An element index.
     */
    int32_t getIndex() const
    {
        return index_;
    }

private:

    /**
     * @brief Links a given node after this.
     *
     * @param node Pointer to linking node.
     */
    void link(LinkedNode<T,A>* node)
    {
        next_->prev_ = node;
        node->next_ = next_;
        next_ = node;
        node->prev_ = this;
    }   

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
    T element_;

};

} // namespace lib
} // namespace eoos
#endif // LIB_LINKEDNODE_HPP_
