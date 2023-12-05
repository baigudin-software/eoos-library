/**
 * @file      lib.CircularList.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_CIRCULARLIST_HPP_
#define LIB_CIRCULARLIST_HPP_

#include "lib.AbstractList.hpp"

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
class CircularList : public AbstractList<T,A>
{
    typedef AbstractList<T,A> Parent;

public:

    using Parent::isConstructed;

    /**
     * @brief Constructor.
     */
    CircularList() 
        : AbstractList<T,A>() {
    }

    /**
     * @brief Constructor.
     *
     * @param illegal An illegal element.
     */
    CircularList(T const illegal) 
        : AbstractList<T,A>(illegal) {
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
    virtual api::ListIterator<T>* getListIterator(int32_t const index=0)
    {
        Iterator<T,A>* it( NULLPTR );
        if( isConstructed() )
        {
            it = new Iterator<T,A>(index, *this);
            if( it != NULLPTR )
            {
                if( !it->isConstructed() )
                {
                    delete it;
                    it = NULLPTR;
                }
            }
        }
        return it;         
    }

protected:

    using Parent::setConstructed;

private:
    
    /**
     * @class Iterator
     * @brief The list iterator.
     *
     * @note This class is implemented in private zone of the list class.
     * For this reason, for fast iteration some tests are skipped.
     * You have to use this class only if it has been constructed.
     *
     * @tparam TT Data type of container element.
     * @tparam AA Heap memory allocator class.
     */
    template <typename TT, class AA>
    class Iterator : public NonCopyable<AA>, public api::ListIterator<TT>
    {
        typedef NonCopyable<AA> Parent;
        typedef CircularList<TT,AA>  List;
        typedef LinkedNode<TT,AA> Node;        

    public:

        /**
         * @brief Constructor.
         *
         * @param index Position in this list.
         * @param list  Reference to self list.
         */
        Iterator(int32_t const index, List& list) 
            : NonCopyable<AA>()
            , api::ListIterator<TT>()
            , list_    (list)
            , count_   (list.getReferenceToCount())
            , last_    (list.getReferenceToLast())
            , illegal_ (list.getReferenceToIllegal())
            , curs_    (NULLPTR)
            , rindex_  (ILLEGAL_INDEX) {
            bool_t const  isConstructed( construct(index) );
            this->setConstructed( isConstructed );
        }

        /**
         * @brief Destructor.
         */
        virtual ~Iterator(){}

        /**
         * @copydoc eoos::api::Object::isConstructed()
         */
        virtual bool_t isConstructed() const ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
        {
            return Parent::isConstructed();
        }

        /**
         * @copydoc eoos::api::ListIterator::add(T const&)
         */
        virtual bool_t add(TT const& element)
        {
            bool_t res( false );
            if( !isModifiedByList() )
            {
                bool_t const wasEmpty( list_.isEmpty() );
                int32_t const index = ( wasEmpty ) ? 0 : curs_->getIndex(); 
                res = list_.add(index, element);
                if( res == true )
                {
                    count_.self++; ///< SCA MISRA-C++:2008 Defected Rule 5-2-10
                    rindex_ = ILLEGAL_INDEX;
                    if( wasEmpty )
                    {
                        curs_ = last_;
                    }
                }
            }            
            return res;
        }

        /**
         * @copydoc eoos::api::Iterator::remove()
         */
        virtual bool_t remove()
        {
            bool_t res( false );
            if( !isModifiedByList() && (rindex_ != ILLEGAL_INDEX) )
            {
                Node* curs( curs_ );            
                if(curs_->getIndex() == rindex_)
                {
                    curs = curs_->getNext();
                }
                res = list_.remove(rindex_);            
                if(res == true)
                {
                    count_.self++; ///< SCA MISRA-C++:2008 Defected Rule 5-2-10
                    rindex_ = ILLEGAL_INDEX;
                    curs_ = ( list_.isEmpty() ) ? NULLPTR : curs;
                }
            }
            return res;
        }

        /**
         * @copydoc eoos::api::ListIterator::getPrevious()
         */
        virtual TT& getPrevious()
        {
            bool_t res( false );
            if( hasPrevious() )
            {
                curs_ = curs_->getPrevious();
                rindex_ = curs_->getIndex();
                res = true;                
            }
            else
            {
                rindex_ = ILLEGAL_INDEX;
                res = false;
            }
            return (res == true ) ? curs_->getElement() : illegal_;
        }

        /**
         * @copydoc eoos::api::ListIterator::getPreviousIndex()
         */
        virtual int32_t getPreviousIndex() const
        {
            int32_t index( api::ListIterator<TT>::ERROR_INDEX );
            if( !isModifiedByList() )
            {
                index = hasPrevious() ? curs_->getPrevious()->getIndex() : -1;
            }                        
            return index;
        }

        /**
         * @copydoc eoos::api::ListIterator::hasPrevious()
         */
        virtual bool_t hasPrevious() const
        {
            bool_t res( false );
            if( !isModifiedByList() )
            {
                if(curs_ != NULLPTR)
                {
                    res = true;
                }
            }
            return res;
        }

        /**
         * @copydoc eoos::api::Iterator::getNext()
         */
        virtual TT& getNext()
        {
            TT* element( &illegal_ );
            if( hasNext() )
            {
                Node* const node( curs_ );
                curs_ = curs_->getNext();
                rindex_ = node->getIndex();
                element = &node->getElement();
            }
            else
            {
                rindex_ = ILLEGAL_INDEX;
            }
            return *element;

        }

        /**
         * @copydoc eoos::api::ListIterator::getNextIndex()
         */
        virtual int32_t getNextIndex() const
        {
            int32_t index( api::ListIterator<TT>::ERROR_INDEX );
            if( !isModifiedByList() )
            {
                index = hasNext() ? curs_->getIndex() : -1;
            }
            return index;
        }

        /**
         * @copydoc eoos::api::Iterator::hasNext()
         */
        virtual bool_t hasNext() const
        {
            bool_t res( false );
            if( !isModifiedByList() )
            {
                if(curs_ != NULLPTR)
                {
                    res = true;
                }
            }
            return res;
        }

        /**
         * @copydoc eoos::api::IllegalValue::getIllegal()
         */
        virtual TT const& getIllegal() const
        {
            return list_.getIllegal();
        }

        /**
         * @copydoc eoos::api::IllegalValue::setIllegal(T const&)
         */
        virtual void setIllegal(TT const& value)
        {
            list_.setIllegal(value);
        }

        /**
         * @copydoc eoos::api::IllegalValue::isIllegal(T const&)
         */
        virtual bool_t isIllegal(TT const& value) const
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
            bool_t res( false );
            if( isConstructed() && list_.isConstructed() )
            {
                if( !list_.isIndexOutOfBounds(index) )
                { 
                    if( !list_.isEmpty() )
                    {
                        if( static_cast<size_t>(index) != list_.getLength() )
                        {
                            curs_ = list_.getNodeByIndex(index);
                            res = true;
                        }
                    }
                    else
                    {
                        res = true;
                    }
                }
            }
            return res;

        }
        
        /**
         * @brief Tests if list was modified by list object.
         *
         * @param true if modified.
         */
        bool_t isModifiedByList() const
        {        
            return count_.list != count_.self;
        }
        
        /**
         * @brief List changing counter.
         */
        struct Counter
        {
            /**
             * @brief Constructor.
             */
            Counter(uint32_t& count)
                : list (count)
                , self (count) {
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
            uint32_t const& list; ///< SCA MISRA-C++:2008 Justified Rule 11-0-1

            /**
             * @brief Quantity of chang made by the iterator.
             */
            uint32_t self; ///< SCA MISRA-C++:2008 Justified Rule 11-0-1

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
        TT& illegal_;

        /**
         * @brief Pointer to current node of this iterator that returned as next element.
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
#endif // LIB_CIRCULARLIST_HPP_
