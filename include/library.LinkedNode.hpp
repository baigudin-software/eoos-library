/** 
 * Element for linked lists.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_LINKED_NODE_HPP_
#define LIBRARY_LINKED_NODE_HPP_

#include "library.Object.hpp"

namespace local
{
    namespace library
    {  
        /** 
         * Primary template implementation.
         *
         * @param T data type of element.
         * @param A heap memory allocator class.
         */
        template <typename T, class A = Allocator>
        class LinkedNode : public library::Object<A>
        {
            typedef library::Object<A> Parent;  
        
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
            virtual bool isConstructed() const
            {
                return this->isConstructed_;
            }        
        
            /**
             * Inserts a new element after this.
             *
             * Method links a node after this and reindexes
             * chain of nodes starts from given node.
             *
             * @param node pointer to inserted node.
             */
            virtual void insertAfter(library::LinkedNode<T,A>* node)
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
            virtual void insertBefore(library::LinkedNode<T,A>* node)
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
            virtual library::LinkedNode<T,A>* getPrevious() const
            {
                return prev_;
            }
            
            /**
             * Returns next element.
             *
             * @return next element.
             */  
            virtual library::LinkedNode<T,A>* getNext() const
            {
                return next_;
            }
        
            /**
             * Returns the element.
             *
             * @return next element.
             */  
            virtual const T& getElement() const
            {
                return element_;
            }
        
            /**
             * Returns the element index.
             *
             * @return element index.
             */  
            virtual int32 getIndex() const
            {
                return index_;
            }
    
        private:
        
            /**
             * Links a given node after this.
             *
             * @param node pointer to linking node.
             */
            void link(library::LinkedNode<T,A>* node)
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
            LinkedNode& operator =(const LinkedNode& obj);
        
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
            int32 index_;
            
            /**
             * Containing element.
             */        
            T element_;
        
        };
    }
}
#endif // LIBRARY_LINKED_NODE_HPP_
