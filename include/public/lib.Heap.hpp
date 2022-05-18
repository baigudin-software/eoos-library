/**
 * @file      lib.Heap.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_HEAP_HPP_
#define LIB_HEAP_HPP_

#include "api.SystemHeap.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class Heap
 * @brief Heap memory.
 *
 * Hardware address for system heap memory has to be aligned to eight. 
 */
class Heap : public api::SystemHeap
{
    typedef Heap Self;

public:

    /**
     * @brief Constructor.
     *
     * @param size Total heap size.
     */
    explicit Heap(size_t size) 
        : api::SystemHeap()
        , data_(size)
        , temp_() {
        const bool_t isConstructed = construct();
        setConstructed( isConstructed );
    }

    /**
     * @brief Constructor.
     *
     * Reference to global interrupt interface pointer is used for
     * a possibility to change a value of that pointer.
     * Until that pointer is NULLPTR golobal interrupt is not used.
     * This gives you a possibility to change using golobal interrupts
     * on fly.
     *
     * @param size   Total heap size.
     * @param toggle Reference to pointer to global interrupts toggle interface.
     */
    Heap(size_t size, api::Toggle*& toggle) 
        : api::SystemHeap()
        , data_  (size, toggle)
        , temp_ () {
        const bool_t isConstructed = construct();
        setConstructed( isConstructed );
    }

    /**
     * @brief Destructor.
     */
    virtual ~Heap()
    {
        data_.key = 0;
    }

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const
    {
        if( data_.key != HEAP_KEY )
        {
            return false;
        }
        if( not getFirstBlock()->isConstructed() )
        {
            return false;
        }
        return  true;
    }

    /**
     * @copydoc eoos::api::Heap::allocate(size_t,void*)
     */
    virtual void* allocate(const size_t size, void* ptr)
    {
        if( not Self::isConstructed() )
        {
            return NULLPTR;
        }
        if( ptr != NULLPTR )
        {
            return ptr;
        }
        const bool_t is = disable();
        ptr = getFirstBlock()->alloc(size);
        enable(is);
        return ptr;
    }

    /**
     * @copydoc eoos::api::Heap::free(void*)
     */
    virtual void free(void* ptr)
    {
        if( ptr == NULLPTR )
        {
            return;
        }
        if( not Self::isConstructed() )
        {
            return;
        }
        const bool_t is = disable();
        heapBlock(ptr)->free();
        enable(is);
    }

    /**
     * @copydoc eoos::api::SystemHeap::setToggle(api::Toggle*&)
     */
    virtual void setToggle(api::Toggle*& toggle)
    {
        data_.toggle = &toggle;
    }

    /**
     * @copydoc eoos::api::SystemHeap::resetToggle()
     */
    virtual void resetToggle()
    {
        data_.toggle = NULLPTR;
    }

    /**
     * @brief Operator new.
     *
     * Function initiates a building of heap memory
     * checks and tests self memory structure data
     * and leads to call the class constructor.
     *
     * @param size Unused.
     * @param ptr  Aligned to eight memory address.
     * @return Address of memory or NULLPTR.
     */
    static void* operator new(size_t, const uintptr_t ptr)
    {
        void* memory;
        void* address = reinterpret_cast< void* >(ptr);
        if(address == NULLPTR)
        {
            // No class constructor call
            memory = address;
        }
        else
        {
            // Create the heap
            memory = create(address);
        }
        return memory;
    }
    
    /**
     * @brief Operator delete.
     */
    static void operator delete(void*, uintptr_t) {}    

    /**
     * @brief Operator delete.
     */
    static void operator delete(void*) {}

private:

    class HeapBlock;

    /**
     * @brief Sets the object constructed flag.
     *
     * @param flag Constructed flag.
     */
    void setConstructed(const bool_t flag)
    {
        if(data_.key == HEAP_KEY)
        {
            data_.key = flag ? HEAP_KEY : 0;
        }
    }

    /**
     * @brief Constructor.
     *
     * @return True if object has been constructed successfully.
     */
    bool_t construct()
    {
        // Crop a size to multiple of eight
        if( (sizeof(HeapBlock) + 16UL) > data_.size )
        {
            return false;
        }
        // Test Heap and HeapBlock structures sizes witch has to be multipled to eight
        if((sizeof(Heap) & 0x7UL) != 0UL)
        {
            return false;
        }
        if((sizeof(HeapBlock) & 0x7UL) != 0UL)
        {
            return false;
        }
        // Test memory
        const uintptr_t addr = reinterpret_cast<uintptr_t>(this) + sizeof(Heap);
        void*  ptr  = reinterpret_cast<void*>(addr);
        if( not isMemoryAvailable(ptr, data_.size) )
        {
            return false;
        }
        // Alloc first heap block
        data_.block = new ( getFirstBlock() ) HeapBlock(this, data_.size);
        return (data_.block != NULLPTR) ? true : false;
    }

    /**
     * @brief Disables a controller.
     *
     * @return An enable source bit value of a controller before function was called.
     */
    bool_t disable() const
    {
        if(data_.toggle == NULLPTR)
        {
            return false;
        }
        api::Toggle* const toggle = *data_.toggle;
        return (toggle != NULLPTR) ? toggle->disable() : false;
    }

    /**
     * @brief Enables a controller.
     *
     * @param status returned status by disable function.
     */
    void enable(const bool_t status) const
    {
        if(data_.toggle == NULLPTR)
        {
            return;
        }
        api::Toggle* const toggle = *data_.toggle;
        if(toggle != NULLPTR)
        {
            toggle->enable(status);
        }
    }

    /**
     * @brief Returns a first heap block address.
     *
     * @return Pointer to heap block.
     */
    HeapBlock* getFirstBlock() const
    {
        const uintptr_t addr = reinterpret_cast<uintptr_t>(this) + sizeof(Heap);
        return reinterpret_cast<HeapBlock*>(addr);
    }

    /**
     * @brief Returns a heap block by user data address.
     *
     * @return Pointer to heap block.
     */
    static HeapBlock* heapBlock(void* const data)
    {
        const uintptr_t addr = reinterpret_cast<uintptr_t>(data) - sizeof(HeapBlock);
        return reinterpret_cast<HeapBlock*>(addr);
    }

    /**
     * @brief Tests memory.
     *
     * @todo normal type casts should be done.
     *
     * @param addr Memory address pointer.
     * @param size Size in byte.
     * @return True if test complete.
     */
    static bool_t isMemoryAvailable(void* const addr, const size_t size)
    {
        size_t mask = static_cast<ucell_t>(-1);
        ucell_t* ptr = reinterpret_cast<ucell_t*>(addr);
        // Value test
        for( size_t i=0UL; i<size; i++)
        {
            ptr[i] = static_cast<ucell_t>(i & mask);
        }
        for( size_t i=0UL; i<size; i++)
        {
            if(ptr[i] != static_cast<ucell_t>(i & mask))
            {
                return false;
            }
        }
        // 0x55 test
        for( size_t i=0UL; i<size; i++)
        {
            ptr[i] = static_cast<ucell_t>(0x55555555UL & mask);
        }
        for( size_t i=0UL; i<size; i++)
        {
            if(ptr[i] != static_cast<ucell_t>(0x55555555UL & mask))
            {
                return false;
            }
        }
        // 0xAA test
        for( size_t i=0UL; i<size; i++)
        {
            ptr[i] = static_cast<ucell_t>(0xAAAAAAAAUL & mask);
        }
        for( size_t i=0UL; i<size; i++)
        {
            if(ptr[i] != static_cast<ucell_t>(0xAAAAAAAAUL & mask))
            {
                return false;
            }
        }
        // Zero test
        for( size_t i=0UL; i<size; i++)
        {
            ptr[i] = 0x00U;
        }
        for( size_t i=0UL; i<size; i++)
        {
            if(ptr[i] != 0x00U)
            {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Allocates memory for heap.
     *
     * Function initiates a building of heap memory
     * checks and tests self memory structure data
     * and leads to call the class constructor.
     *
     * @param ptr Aligned to eight memory address.
     * @return Address of memory or NULLPTR.
     */
    static void* create(void* ptr)
    {
        // Size of this class has to be multipled to eight
        if((sizeof(Heap) & 0x7UL) != 0UL)
        {
            ptr = NULLPTR;
        }
        // Testing memory for self structure data
        //
        // @todo copy constructor of the Heap class for
        // temporary copying the tested memory to that
        // class. This way would help to restore original
        // memory data if the test were failed.
        if( not isMemoryAvailable(ptr, sizeof(Heap)) )
        {
            ptr = NULLPTR;
        }
        // Memory address has to be aligned to eight
        if( (reinterpret_cast<uintptr_t>(ptr) & 0x7UL) != 0UL )
        {
            ptr = NULLPTR;
        }
        return ptr;
    }
    
    /**
     * @copydoc eoos::Object::Object(const Object&)
     */
    Heap(Heap const&); ///< SCA MISRA-C++:2008 Justified Rule 3-2-2

    /**
     * @copydoc eoos::Object::operator=(const Object&)
     */
    Heap& operator=(Heap const&); ///< SCA MISRA-C++:2008 Justified Rule 3-2-2
    
    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(Object&&)
     */       
    Heap(Heap&&) noexcept = delete; 
    
    /**
     * @copydoc eoos::Object::operator=(Object&&)
     */
    Heap& operator=(Heap&&) noexcept = delete;
    
    #endif // EOOS_CPP_STANDARD >= 2011    

    /**
     * @struct Aligner<S>
     * @brief Heap class aligner aligns that to eight.
     *
     * @note If given S is already multiple 8, the class size will be 8 bytes.
     *
     * @tparam S Size of Heap class.
     */
    template <size_t S>
    struct Aligner
    {

    public:

        /**
         * @brief Constructor.
         */
        Aligner()
        {
            #ifdef EOOS_DEBUG
            for(size_t i=0; i<SIZE; i++) val_[i] = 0x0AUL;
            #endif
        }

        /**
         * @brief Destructor.
         */
       ~Aligner(){}

    private:

        /**
         * @brief Aligning data size.
         */
		static const size_t SIZEOF = S;

        /**
         * @brief Aligning data size.
         */
		static const size_t MASK = ~0x7UL;

        /**
         * @brief Aligning data size.
         */
        static const size_t SIZE = ( ( SIZEOF & MASK ) + 0x8UL ) - SIZEOF;

        /**
         * @brief Temp array.
         */
        ucell_t val_[SIZE];

    };

    /**
     * @class VirtualTable
     * @brief Contains a Virtual Function Table only.
     *
     * Probably, the solution of using this empty class
     * is not delicate, but it works for understanding
     * about size of a virtual function table of this Heap class.
     *
     * @note This uint64_t variable of the class is used, because some compilers
     * might put 64 bit variable to aligned 8 memory address. Therefore, size of classes
     * with 32 bit pointer to virtual table and one 64 bit variable is 16 bytes.
     */
    class VirtualTable : public api::Heap{uint64_t temp;};

    /**
     * @class HeapBlock
     * @brief Heap memory block.
     *
     * The class data has to be aligned to 8.
     */
    class HeapBlock
    {

    public:

        /**
         * @brief Constructor.
         *
         * @param heap Pointer to heap class.
         * @param size Size of byte given to this new block.
         */
        HeapBlock(api::Heap* heap, size_t size) 
            : heap_(heap)
            , prev_(NULLPTR)
            , next_(NULLPTR)
            , attr_(0)
            , size_(size - sizeof(HeapBlock))
            , key_(BLOCK_KEY) {
        }

        /**
         * @brief Destructor.
         */
       ~HeapBlock()
        {
        }

        /**
         * @brief Tests if this object has been constructed.
         *
         * @return True if object has been constructed successfully.
         */
        bool_t isConstructed() const
        {
            return (key_ == BLOCK_KEY) ? true : false;
        }

        /**
         * @brief Allocates a memory block.
         *
         * @param size Size in byte.
         * @return Pointer to an allocated memory.
         */
        void* alloc(size_t size)
        {
            if(size == 0UL)
            {
                return NULLPTR;
            }
            // Align a size to 8 byte boudary
            if((size & 0x7UL) != 0UL)
            {
                size = (size & ~0x7UL) + 0x8UL;
            }
            HeapBlock* curr = this;
            while(curr != NULLPTR)
            {
                if(curr->isUsed())
                {
                    curr = curr->next_;
                    continue;
                }
                if(curr->size_ < size)
                {
                    curr = curr->next_;
                    continue;
                }
                break;
            }
            if(curr == NULLPTR)
            {
                return NULLPTR;
            }
            // Has required memory size for data and a new heap block
            if( curr->size_ >= (size + sizeof(HeapBlock)) )
            {
                HeapBlock* next = new ( curr->next(size) ) HeapBlock(heap_, curr->size_ - size);
                if(next == NULLPTR)
                {
                    return NULLPTR;
                }
                next->next_ = curr->next_;
                next->prev_ = curr;
                if(next->next_ != NULLPTR)
                {
                    next->next_->prev_ = next;
                }
                curr->next_ = next;
                curr->size_ = size;
            }
            curr->attr_ |= ATTR_USED;
            return curr->data();
        }

        /**
         * @brief Frees allocated memory by this block.
         */
        void free()
        {
            if( not canDelete() )
            {
                return;
            }
            uint32_t sibling = 0UL;
            if( prev_ != NULLPTR )
            {
                if( not prev_->isUsed() )
                {
                    sibling |= PREV_FREE;
                }
            }
            if( next_ != NULLPTR )
            {
                if( not next_->isUsed() )
                {
                    sibling |= NEXT_FREE;
                }
            }
            switch(sibling)
            {
                case PREV_FREE | NEXT_FREE:
                {
                    prev_->size_ += ( 2UL * sizeof(HeapBlock) ) + size_ + next_->size_;
                    prev_->next_ = next_->next_;
                    if(prev_->next_ != NULLPTR)
                    {
                        prev_->next_->prev_ = prev_;
                    }
                    break;
                }
                case PREV_FREE:
                {
                    prev_->size_ += sizeof(HeapBlock) + size_;
                    prev_->next_ = next_;
                    if(next_ != NULLPTR)
                    {
                        next_->prev_ = prev_;
                    }
                    break;
                }
                case NEXT_FREE:
                {
                    size_ += sizeof(HeapBlock) + next_->size_;
                    next_ = next_->next_;
                    if(next_ != NULLPTR)
                    {
                        next_->prev_ = this;
                    }
                    attr_ &= MASK_UNUSED;
                    break;
                }
                default:
                {
                    attr_ &= MASK_UNUSED;
                    break;
                }
            }
        }

        /**
         * @brief Operator new.
         *
         * @param size Unused.
         * @param ptr  Address of memory.
         * @return Address of memory.
         */
        static void* operator new(size_t, void* const ptr)
        {
            void* memory;
            do
            {
                // Size of this class must be multipled to eight
                if((sizeof(HeapBlock) & 0x7UL) != 0UL)
                {
                    memory = NULLPTR;
                    break;
                }
                // The passed address must be multipled to eight
                if((reinterpret_cast<uintptr_t>(ptr) & 0x7UL) != 0UL)
                {
                    memory = NULLPTR;
                    break;
                }
                memory = ptr;
            }
            while(false);
            return memory;
        }
        
        /**
         * @brief Operator delete.
         */
        static void operator delete(void*, void*) {}

    private:

        /**
         * @brief Tests if this memory block is available for deleting.
         *
         * @return True if it may be deleted.
         */
        bool_t canDelete() const
        {
            if( not isConstructed() )
            {
                return false;
            }
            if( not heap_->isConstructed() )
            {
                return false;
            }
            return true;
        }

        /**
         * @brief Tests if this memory block is available.
         *
         * @return True if memory block is available.
         */
        bool_t isUsed() const
        {
            return ( (attr_ & ATTR_USED) != 0UL ) ? true : false;
        }

        /**
         * @brief Returns an address to data of this block.
         *
         * @return Pointer to memory.
         */
        void* data()
        {
            const uintptr_t addr = reinterpret_cast<uintptr_t>(this) + sizeof(HeapBlock);
            return reinterpret_cast<void*>(addr);
        }

        /**
         * @brief Returns an address to next block.
         *
         * @return PSinter to memory.
         */
        void* next(const size_t size)
        {
            const uintptr_t addr = reinterpret_cast<uintptr_t>(this) + sizeof(HeapBlock) + size;
            return reinterpret_cast<void*>(addr);
        }

        /**
         * @copydoc eoos::Object::Object(const Object&)
         */
        HeapBlock(HeapBlock const&); ///< SCA MISRA-C++:2008 Justified Rule 3-2-2
    
        /**
         * @copydoc eoos::Object::operator=(const Object&)
         */
        HeapBlock& operator=(HeapBlock const&); ///< SCA MISRA-C++:2008 Justified Rule 3-2-2
        
        #if EOOS_CPP_STANDARD >= 2011
    
        /**
         * @copydoc eoos::Object::Object(Object&&)
         */       
        HeapBlock(HeapBlock&&) noexcept = delete; 
        
        /**
         * @copydoc eoos::Object::operator=(Object&&)
         */
        HeapBlock& operator=(HeapBlock&&) noexcept = delete;
        
        #endif // EOOS_CPP_STANDARD >= 2011    

        /**
         * @brief Heap block definition key.
         */
        static const size_t BLOCK_KEY = 0x20150515UL;

        /**
         * @brief Block is used.
         */
        static const uint32_t ATTR_USED = 0x00000001UL;

        /**
         * @brief Next block is free.
         */
        static const uint32_t NEXT_FREE = 0x00000001UL;

        /**
         * @brief Previous block is free.
         */
        static const uint32_t PREV_FREE = 0x00000002UL;

        /**
         * @brief Mask block is unused.
         */
        static const uint32_t MASK_UNUSED = 0xFFFFFFFEUL;

        /**
         * @brief Heap page of this block.
         */
        api::Heap* heap_;

        /**
         * @brief Next block.
         */
        HeapBlock* prev_;

        /**
         * @brief Previous block.
         */
        HeapBlock* next_;

        /**
         * @brief Attributes of this block.
         */
        uint32_t attr_;

        /**
         * @brief Size in byte of this block.
         */
        size_t size_;

        /**
         * @brief Heap block definition key.
         */
        size_t key_;

    };

    /**
     * @struct HeapData
     * @brief Heap data.
     *
     * This structure is needed for aligning heap data or otherwise
     * this Heap class can not de aligned because it is incompleted.
     */
    struct HeapData
    {
        /**
         * @brief Constructor.
         *
         * @param isize Total heap size.
         */
        explicit HeapData(size_t isize) 
            : block(NULLPTR)
            , toggle(NULLPTR)
            , size(0)
            , key(HEAP_KEY) {
            size = (isize & ~0x7UL) - sizeof(Heap);
        }

        /**
         * @brief Constructor.
         *
         * @param isize   Total heap size.
         * @param itoggle Reference to pointer to global interrupts interface.
         */
        HeapData(size_t isize, api::Toggle*& itoggle) 
            : block(NULLPTR)
            , toggle(&itoggle)
            , size(0)
            , key(HEAP_KEY){
            size = (isize & ~0x7UL) - sizeof(Heap);
        }

        /**
         * @brief Destructor.
         */
       ~HeapData(){}

        /**
         * @brief First memory block of heap page memory.
         */
        HeapBlock* block; ///< SCA MISRA-C++:2008 Justified Rule 11-0-1

        /**
         * @brief Threads switching off key.
         *
         * This interface controls a global thread switch off key
         * by toggle interface. That interface has to disable
         * a changing thread context. The most useful case is to give
         * a global interrupts toggle interface.
         */
        api::Toggle** toggle; ///< SCA MISRA-C++:2008 Justified Rule 11-0-1

        /**
         * @brief Actual size of heap.
         */
        size_t size; ///< SCA MISRA-C++:2008 Justified Rule 11-0-1

        /**
         * @brief Heap page memory definition key.
         */
        int32_t key; ///< SCA MISRA-C++:2008 Justified Rule 11-0-1

    private:

        /**
         * @copydoc eoos::Object::Object(const Object&)
         */
        HeapData(HeapData const&); ///< SCA MISRA-C++:2008 Justified Rule 3-2-2
    
        /**
         * @copydoc eoos::Object::operator=(const Object&)
         */
        HeapData& operator=(HeapData const&); ///< SCA MISRA-C++:2008 Justified Rule 3-2-2
        
        #if EOOS_CPP_STANDARD >= 2011
    
        /**
         * @copydoc eoos::Object::Object(Object&&)
         */       
        HeapData(HeapData&&) noexcept = delete; 
        
        /**
         * @copydoc eoos::Object::operator=(Object&&)
         */
        HeapData& operator=(HeapData&&) noexcept = delete;
        
        #endif // EOOS_CPP_STANDARD >= 2011    

    };

    /**
     * @brief Size of this Heap class without aligned data.
     */
    static const size_t SIZEOF_HEAP = ( sizeof(HeapData) + sizeof(VirtualTable) ) - sizeof(uint64_t);

    /**
     * @brief Heap page memory definition key.
     */
    static const int32_t HEAP_KEY = 0x19811019;

    /**
     * @brief Data of this heap.
     */
    HeapData data_;

    /**
     * @brief Aligning data.
     */
    Aligner<SIZEOF_HEAP> temp_;

};

} // namespace lib
} // namespace eoos
#endif // LIB_HEAP_HPP_
