/**
 * Doubly linked list.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_LINKED_LIST_HPP_
#define LIB_LINKED_LIST_HPP_

#include "lib.AbstractLinkedList.hpp"

namespace eoos
{
    namespace lib
    {
        /**
         * Primary template implementation.
         *
         * @param T data type of container element.
         * @param A heap memory allocator class.
         */
        template <typename T, class A = Allocator>
        class LinkedList : public AbstractLinkedList<T,A>
        {
            typedef AbstractLinkedList<T,A>  Parent;
            typedef LinkedNode<T,A>          Node;

        public:

            /**
             * Constructor.
             */
            LinkedList() : Parent()
            {
            }

            /**
             * Constructor.
             *
             * NOTE: A passed element must be copied to an internal data structure of
             * this class by calling a copy constructor so that the element
             * might be invalidated after the function called.
             *
             * @param illegal - an illegal element.
             */
            LinkedList(const T& illegal) : Parent(illegal)
            {
            }

            /**
             * Destructor.
             */
            virtual ~LinkedList()
            {
            }

            /**
             * Returns a list iterator of this list elements.
             *
             * @param index start position in this list.
             * @return pointer to new list iterator.
             */
            virtual api::ListIterator<T>* getListIterator(const int32_t index)
            {
                if( not Parent::isConstructed() )
                {
                    return NULLPTR;
                }
                Iterator* const iterator = new Iterator(index, *this);
                if(iterator != NULLPTR && iterator->isConstructed())
                {
                    return iterator;
                }
                delete iterator;
                return NULLPTR;
            }

        private:

            /**
             * Copy constructor.
             *
             * @param obj reference to source object.
             */
            LinkedList(const LinkedList& obj);

            /**
             * Assignment operator.
             *
             * @param obj reference to source object.
             * @return reference to this object.
             */
            LinkedList& operator=(const LinkedList& obj);

            /**
             * The list iterator.
             *
             * This class is implemented in private zone of the list class.
             * For this reason, for fast iteration some tests are skipped.
             * You have to use this class only if it has been constructed.
             */
            class Iterator : public Object<A>, public api::ListIterator<T>
            {
                typedef Iterator Self;
                typedef ::eoos::lib::Object<A> Parent;
                typedef LinkedList<T,A> List;

            public:

                /**
                 * Constructor.
                 *
                 * @param index position in this list.
                 * @param list  reference to self list.
                 */
                Iterator(const int32_t index, List& list) :
                    list_    (list),
                    count_   (list.getReferenceToCount()),
                    last_    (list.getReferenceToLast()),
                    illegal_ (list.getReferenceToIllegal()),
                    curs_    (NULLPTR),
                    rindex_  (ILLEGAL_INDEX){
                    const bool_t isConstructed = construct(index);
                    this->setConstructed( isConstructed );
                }

                /**
                 * Destructor.
                 */
                virtual ~Iterator(){}

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
                 * Inserts the specified element into the list.
                 *
                 * @param element inserting element.
                 * @return true if element is added.
                 */
                virtual bool_t add(const T& element)
                {
                    if(count_.list != count_.self)
                    {
                        return false;
                    }
                    if(list_.add(getNextIndex(), element) == false)
                    {
                        return false;
                    }
                    count_.self++;
                    rindex_ = ILLEGAL_INDEX;
                    return true;
                }

                /**
                 * Removes the last element returned by this iterator.
                 *
                 * @return true if an element is removed successfully.
                 */
                virtual bool_t remove()
                {
                    Node* curs;
                    if(count_.list != count_.self)
                    {
                        return false;
                    }
                    if(rindex_ == ILLEGAL_INDEX)
                    {
                        return false;
                    }
                    if(curs_->getIndex() != rindex_)
                    {
                        curs = curs_;
                    }
                    else
                    {
                        curs = curs_ != last_ ? curs_->getNext() : NULLPTR;
                    }
                    if(list_.remove(rindex_) == false)
                    {
                        return false;
                    }
                    count_.self++;
                    rindex_ = ILLEGAL_INDEX;
                    curs_ = curs;
                    return true;
                }

                /**
                 * Returns previous element and advances the cursor backwards.
                 *
                 * @return reference to element.
                 */
                virtual T& getPrevious() const
                {
                    if( not hasPrevious() )
                    {
                        return illegal_;
                    }
                    curs_ = curs_ == NULLPTR ? last_ : curs_->getPrevious();
                    rindex_ = curs_->getIndex();
                    return curs_->getElement();
                }

                /**
                 * Returns the index of the element that would be returned by a subsequent call to getPrevious().
                 *
                 * @return index of the previous element or -1 if the list iterator is at the beginning of the list.
                 */
                virtual int32_t getPreviousIndex() const
                {
                    if( not hasPrevious() )
                    {
                        return -1;
                    }
                    return curs_ == NULLPTR ? last_->getIndex() : curs_->getPrevious()->getIndex();
                }

                /**
                 * Tests if this iteration may return a previous element.
                 *
                 * @return true if previous element is had.
                 */
                virtual bool_t hasPrevious() const
                {
                    if(count_.list != count_.self)
                    {
                        return false;
                    }
                    if(last_ == NULLPTR)
                    {
                        return false;
                    }
                    if(curs_->getPrevious() == last_)
                    {
                        return false;
                    }
                    return true;
                }

                /**
                 * Returns next element and advances the cursor position.
                 *
                 * @return reference to element.
                 */
                virtual T& getNext() const
                {
                    if( not hasNext() )
                    {
                        return illegal_;
                    }
                    Node* const node = curs_;
                    curs_ = curs_ != last_ ? curs_->getNext() : NULLPTR;
                    rindex_ = node->getIndex();
                    return node->getElement();
                }

                /**
                 * Returns the index of the element that would be returned by a subsequent call to getNext().
                 *
                 * @return index of the next element or list size if the list iterator is at the end of the list.
                 */
                virtual int32_t getNextIndex() const
                {
                    return hasNext() ? curs_->getIndex() : list_.getLength();
                }

                /**
                 * Tests if this iteration may return a next element.
                 *
                 * @return true if next element is had.
                 */
                virtual bool_t hasNext() const
                {
                    if(count_.list != count_.self)
                    {
                        return false;
                    }
                    if(curs_ == NULLPTR)
                    {
                        return false;
                    }
                    return true;
                }

                /**
                 * Returns illegal element which will be returned as error value.
                 *
                 * If illegal value is not set method returns uninitialized variable.
                 *
                 * @return illegal element.
                 */
                virtual T& getIllegal() const
                {
                    return list_.getIllegal();
                }

                /**
                 * Sets illegal element which will be returned as error value.
                 *
                 * @param value illegal value.
                 */
                virtual void setIllegal(const T& value)
                {
                    list_.setIllegal(value);
                }

                /**
                 * Tests if given value is an illegal.
                 *
                 * @param value testing value.
                 * @param true if value is an illegal.
                 */
                virtual bool_t isIllegal(const T& value) const
                {
                    return list_.isIllegal(value);
                }

            private:

                /**
                 * Constructor.
                 *
                 * @param index position in this list.
                 */
                bool_t construct(const int32_t index)
                {
                    if( not Self::isConstructed() )
                    {
                        return false;
                    }
                    if( not list_.isConstructed() )
                    {
                        return false;
                    }
                    if( list_.isIndexOutOfBounds(index) )
                    {
                        return false;
                    }
                    curs_ = list_.getNodeByIndex(index);
                    return true;
                }

                /**
                 * Copy constructor.
                 *
                 * @param obj reference to source object.
                 */
                Iterator(const Iterator& obj);

                /**
                 * Assignment operator.
                 *
                 * @param obj reference to source object.
                 * @return reference to this object.
                 */
                Iterator& operator=(const Iterator& obj);

                /**
                 * List changing counter.
                 */
                struct Counter
                {
                    /**
                     * Constructor.
                     */
                    Counter(int32_t& count) :
                        list (count),
                        self (count){
                    }

                    /**
                     * Destructor.
                     */
                   ~Counter()
                    {
                    }

                    /**
                     * Quantity of chang made by iterating list.
                     */
                    const int32_t& list;

                    /**
                     * Quantity of chang made by the iterator.
                     */
                    int32_t self;

                };

                /**
                 * Illegal iterator index
                 */
                static const int32_t ILLEGAL_INDEX = -1;

                /**
                 * The list of this iterator.
                 */
                List& list_;

                /**
                 * Number of changes in the iterator list.
                 */
                Counter count_;

                /**
                 * Last node of the iterator list.
                 */
                Node*& last_;

                /**
                 * Illegal value of the iterator list.
                 */
                T& illegal_;

                /**
                 * Pointer to current node of this iterator.
                 */
                mutable Node* curs_;

                /**
                 * Index of element of list which can be removed by remove method.
                 */
                mutable int32_t rindex_;

            };
        };
    }
}
#endif // LIB_LINKED_LIST_HPP_
