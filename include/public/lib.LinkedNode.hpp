/**
 * Element for linked lists.
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
         * Primary template implementation.
         *
         * @param T data type of element.
         * @param A heap memory allocator class.
         */
        template <typename T, class A = Allocator>
        class LinkedNode : public Object<A>
        {
            typedef LinkedNode<T,A> Self;
            typedef ::eoos::lib::Object<A> Parent;

        public:

            /**
             * Constructor.
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
             * Destructor.
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
             * Tests if this object has been constructed.
             *
             * @return true if object has been constructed successfully.
             */
            virtual bool_t isConstructed() const
            {
                return Parent::isConstructed();
            }

            /**
             * Inserts a new element after this.
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
             * Inserts a new element after this.
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
             * Returns previous element.
             *
             * @return previous element.
             */
            virtual LinkedNode<T,A>* getPrevious() const
            {
                return prev_;
            }

            /**
             * Returns next element.
             *
             * @return next element.
             */
            virtual LinkedNode<T,A>* getNext() const
            {
                return next_;
            }

            /**
             * Returns the element.
             *
             * @return next element.
             */
            virtual T& getElement() const
            {
                return element_;
            }

            /**
             * Returns the element index.
             *
             * @return element index.
             */
            virtual int32_t getIndex() const
            {
                return index_;
            }

        private:

            /**
             * Links a given node after this.
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
             * Copy constructor.
             *
             * @param obj reference to source object.
             */
            LinkedNode(const LinkedNode& obj);

            /**
             * Assignment operator.
             *
             * @param obj reference to source object.
             * @return reference to this object.
             */
            LinkedNode& operator=(const LinkedNode& obj);

            /**
             * Previous node.
             */
            LinkedNode* prev_;

            /**
             * Next node.
             */
            LinkedNode* next_;

            /**
             * Index of the node.
             */
            int32_t index_;

            /**
             * Containing element.
             */
            mutable T element_;

        };
    }
}
#endif // LIB_LINKED_NODE_HPP_
