/**
 * @file      lib.LinkedList.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_LINKEDLIST_HPP_
#define LIB_LINKEDLIST_HPP_

#include "lib.AbstractLinkedList.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class LinkedList<T,A>
 * @brief Doubly linked list.
 *
 * @tparam T Data type of container element.
 * @tparam A Heap memory allocator class.
 */
template <typename T, class A = Allocator>
class LinkedList : public AbstractLinkedList<T,A>
{
    typedef AbstractLinkedList<T,A>  Parent;
    typedef LinkedNode<T,A>          Node;

public:

    /**
     * @brief Constructor.
     */
    LinkedList() : Parent()
    {
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
    LinkedList(const T& illegal) : AbstractLinkedList<T,A>(illegal)
    {
    }

    /**
     * @brief Destructor.
     */
    virtual ~LinkedList()
    {
    }

    /**
     * @copydoc eoos::api::List::getListIterator(int32_t)
     */
    virtual api::ListIterator<T>* getListIterator(const int32_t index)
    {
        if( not Parent::isConstructed() )
        {
            return NULLPTR;
        }
        Iterator* const iterator = new Iterator(index, *this);
        if( iterator != NULLPTR )
        {
            if( iterator->isConstructed() )
            {
                return iterator;
            }
        }
        delete iterator;
        return NULLPTR;
    }

private:

    /**
     * @class Iterator
     * @brief The list iterator.
     *
     * @note This class is implemented in private zone of the list class.
     * For this reason, for fast iteration some tests are skipped.
     * You have to use this class only if it has been constructed.
     */
    class Iterator : public NonCopyable<A>, public api::ListIterator<T>
    {
        typedef Iterator Self;
        typedef NonCopyable<A> Parent;
        typedef LinkedList<T,A> List;

    public:

        /**
         * @brief Constructor.
         *
         * @param index Position in this list.
         * @param list  Reference to self list.
         */
        Iterator(int32_t const index, List& list) : NonCopyable<A>(), api::ListIterator<T>(),
            list_    (list),
            count_   (list.getReferenceToCount()),
            last_    (list.getReferenceToLast()),
            illegal_ (list.getReferenceToIllegal()),
            curs_    (NULLPTR),
            rindex_  (ILLEGAL_INDEX){
            bool_t const isConstructed = construct(index);
            this->setConstructed( isConstructed );
        }

        /**
         * @brief Destructor.
         */
        virtual ~Iterator(){}

        /**
         * @copydoc eoos::api::Object::isConstructed()
         */
        virtual bool_t isConstructed() const ///< SCA MISRA-C++:2008 Justified Rule 10-3-1 and Defected Rule 9-3-3
        {
            return Parent::isConstructed();
        }

        /**
         * @copydoc eoos::api::ListIterator::add(const T&)
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
            count_.self++; ///< SCA MISRA-C++:2008 Defected Rule 5-2-10
            rindex_ = ILLEGAL_INDEX;
            return true;
        }

        /**
         * @copydoc eoos::api::Iterator::remove()
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
                curs = (curs_ != last_) ? curs_->getNext() : NULLPTR;
            }
            if(list_.remove(rindex_) == false)
            {
                return false;
            }
            count_.self++; ///< SCA MISRA-C++:2008 Defected Rule 5-2-10
            rindex_ = ILLEGAL_INDEX;
            curs_ = curs;
            return true;
        }

        /**
         * @copydoc eoos::api::ListIterator::getPrevious()
         */
        virtual T& getPrevious()
        {
            if( not hasPrevious() )
            {
                return illegal_; ///< SCA MISRA-C++:2008 Justified Rule 9-3-2
            }
            curs_ = (curs_ == NULLPTR) ? last_ : curs_->getPrevious();
            rindex_ = curs_->getIndex();
            return curs_->getElement();
        }

        /**
         * @copydoc eoos::api::ListIterator::getPreviousIndex()
         */
        virtual int32_t getPreviousIndex() const
        {
            if( not hasPrevious() )
            {
                return -1;
            }
            return (curs_ == NULLPTR) ? last_->getIndex() : curs_->getPrevious()->getIndex();
        }

        /**
         * @copydoc eoos::api::ListIterator::hasPrevious()
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
         * @copydoc eoos::api::Iterator::getNext()
         */
        virtual T& getNext()
        {
            if( not hasNext() )
            {
                return illegal_; ///< SCA MISRA-C++:2008 Justified Rule 9-3-2
            }
            Node* const node = curs_;
            curs_ = (curs_ != last_) ? curs_->getNext() : NULLPTR;
            rindex_ = node->getIndex();
            return node->getElement();
        }

        /**
         * @copydoc eoos::api::ListIterator::getNextIndex()
         */
        virtual int32_t getNextIndex() const
        {
            return hasNext() ? curs_->getIndex() : list_.getLength();
        }

        /**
         * @copydoc eoos::api::Iterator::hasNext()
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
         * @copydoc eoos::api::IllegalValue::getIllegal()
         */
        virtual T const& getIllegal() const
        {
            return list_.getIllegal();
        }

        /**
         * @copydoc eoos::api::IllegalValue::setIllegal(const T&)
         */
        virtual void setIllegal(const T& value)
        {
            list_.setIllegal(value);
        }

        /**
         * @copydoc eoos::api::IllegalValue::isIllegal(const T&)
         */
        virtual bool_t isIllegal(const T& value) const
        {
            return list_.isIllegal(value);
        }

    private:

        /**
         * @brief Constructor.
         *
         * @param index Position in this list.
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
         * @struct Counter
         * @brief List changing counter.
         */
        struct Counter
        {
            /**
             * @brief Constructor.
             */
            Counter(int32_t& count) :
                list (count),
                self (count){
            }

            /**
             * @brief Destructor.
             */
           ~Counter()
            {
            }

            /**
             * @brief Quantity of chang made by iterating list.
             */
            const int32_t& list; ///< SCA MISRA-C++:2008 Justified Rule 11-0-1

            /**
             * @brief Quantity of chang made by the iterator.
             */
            int32_t self; ///< SCA MISRA-C++:2008 Justified Rule 11-0-1

        };

        /**
         * @brief Illegal iterator index
         */
        static const int32_t ILLEGAL_INDEX = -1;

        /**
         * @brief The list of this iterator.
         */
        List& list_;

        /**
         * @brief Number of changes in the iterator list.
         */
        Counter count_;

        /**
         * @brief Last node of the iterator list.
         */
        Node*& last_;

        /**
         * @brief Illegal value of the iterator list.
         */
        T& illegal_;

        /**
         * @brief Pointer to current node of this iterator.
         */
        Node* curs_;

        /**
         * @brief Index of element of list which can be removed by remove function.
         */
        int32_t rindex_;

    };
};

} // namespace lib
} // namespace eoos
#endif // LIB_LINKEDLIST_HPP_
