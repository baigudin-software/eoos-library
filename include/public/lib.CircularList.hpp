/**
 * @file      lib.CircularList.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2021, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_CIRCULAR_LIST_HPP_
#define LIB_CIRCULAR_LIST_HPP_

#include "lib.AbstractLinkedList.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class CircularList<T,A>
 * @brief Circular doubly linked list.
 *
 * @tparam T Data type of container element.
 * @tparam A Heap memory allocator class.
 */
template <typename T, class A = Allocator>
class CircularList : public AbstractLinkedList<T,A>
{
    typedef AbstractLinkedList<T,A>  Parent;
    typedef LinkedNode<T,A>          Node;

public:

    /**
     * @brief Constructor.
     */
    CircularList() : Parent()
    {
    }

    /**
     * @brief Constructor.
     *
     * @param illegal An illegal element.
     */
    CircularList(const T illegal) : Parent(illegal)
    {
    }

    /**
     * @brief Destructor.
     */
    virtual ~CircularList()
    {
    }

    /**
     * @copydoc eoos::api::List::getListIterator(int32_t)
     */
    virtual api::ListIterator<T>* getListIterator(const int32_t index)
    {
        if( not this->isConstructed_ )
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
     * @copydoc eoos::Object::Object(const Object&)
     */
    CircularList(const CircularList& obj);

    /**
     * @copydoc eoos::Object::operator=(const Object&)
     */
    CircularList& operator=(const CircularList& obj);
    
    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(const Object&&)
     */       
    CircularList(CircularList&& obj) noexcept = delete; 
    
    /**
     * @copydoc eoos::Object::operator=(const Object&&)
     */
    CircularList& operator=(CircularList&& obj) noexcept = delete;
    
    #endif // EOOS_CPP_STANDARD >= 2011
    
    /**
     * @class Iterator
     * @brief The list iterator.
     *
     * @note This class is implemented in private zone of the list class.
     * For this reason, for fast iteration some tests are skipped.
     * You have to use this class only if it has been constructed.
     */
    class Iterator : public Object<A>, public api::ListIterator<T>
    {
        typedef ::eoos::lib::Object<A> Parent;
        typedef CircularList<T,A>  List;

    public:

        /**
         * @brief Constructor.
         *
         * @param index Position in this list.
         * @param list  Reference to self list.
         */
        Iterator(const int32_t index, const List& list) :
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
         * @brief Destructor.
         */
        virtual ~Iterator(){}

        /**
         * @copydoc eoos::api::Object::isConstructed()
         */
        virtual bool_t isConstructed() const
        {
            return this->isConstructed_;
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
            Node* const last = last_;
            if(list_.add(getNextIndex(), element) == false)
            {
                return false;
            }
            count_.self++;
            rindex_ = ILLEGAL_INDEX;
            if(last == NULLPTR)
            {
                curs_ = last_;
            }
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
                curs = curs_->getNext();
            }
            if(list_.remove(rindex_) == false)
            {
                return false;
            }
            count_.self++;
            rindex_ = ILLEGAL_INDEX;
            curs_ = last_ != NULLPTR ? curs : NULLPTR;
            return true;
        }

        /**
         * @copydoc eoos::api::ListIterator::getPrevious()
         */
        virtual const T& getPrevious() const
        {
            if( not hasPrevious())
            {
                return illegal_;
            }
            curs_ = curs_->getPrevious();
            rindex_ = curs_->index();
            return curs_->getElement();
        }

        /**
         * @copydoc eoos::api::ListIterator::getPreviousIndex()
         */
        virtual int32_t getPreviousIndex() const
        {
            return hasPrevious() ? curs_->getPrevious()->getIndex() : -1;
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
            if(curs_ == NULLPTR)
            {
                return false;
            }
            return true;
        }

        /**
         * @copydoc eoos::api::Iterator::getNext()
         */
        virtual const T& getNext() const
        {
            if( not hasNext() )
            {
                return illegal_;
            }
            Node* const node = curs_;
            curs_ = curs_->getNext();
            rindex_ = node->getIndex();
            return node->getElement();
        }

        /**
         * @copydoc eoos::api::ListIterator::getNextIndex()
         */
        virtual int32_t getNextIndex() const
        {
            return hasNext() ? curs_->getIndex() : 0;
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
        virtual const T& getIllegal() const
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
         * @param index position in this list.
         */
        bool_t construct(int32_t index)
        {
            if( not this->isConstructed_ )
            {
                return false;
            }
            else if( not list_.isConstructed())
            {
                return false;
            }
            else if(list_.isIndexOutOfBounds(index))
            {
                return false;
            }
            else if(index == last_->getIndex() + 1)
            {
                index = 0;
            }
            else
            {
            }
            curs_ = list_.getNodeByIndex(index);
            return true;
        }

        /**
         * @copydoc eoos::Object::Object(const Object&)
         */
        Iterator(const Iterator& obj);
    
        /**
         * @copydoc eoos::Object::operator=(const Object&)
         */
        Iterator& operator=(const Iterator& obj);
        
        #if EOOS_CPP_STANDARD >= 2011
    
        /**
         * @copydoc eoos::Object::Object(const Object&&)
         */       
        Iterator(Iterator&& obj) noexcept = delete; 
        
        /**
         * @copydoc eoos::Object::operator=(const Object&&)
         */
        Iterator& operator=(Iterator&& obj) noexcept = delete;
        
        #endif // EOOS_CPP_STANDARD >= 2011
        
        /**
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
            const int32_t& list;

            /**
             * @brief Quantity of chang made by the iterator.
             */
            int32_t self;

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
         * @brief Index of element of list which can be removed by remove method.
         */
        int32_t rindex_;

    };
};

} // namespace lib
} // namespace eoos
#endif // LIB_CIRCULAR_LIST_HPP_
