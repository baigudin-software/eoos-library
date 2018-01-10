/** 
 * The library memory allocator.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2018, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#include "library.Allocator.hpp"

namespace library
{
    /**
     * Allocates memory.
     *
     * @param size number of bytes to allocate.
     * @return allocated memory address or a null pointer.
     */    
    void* Allocator::allocate(size_t size)
    {
        return heap_ != NULL ? heap_->allocate(size, NULL) : NULL;
    }
    
    /**
     * Frees an allocated memory.
     *
     * @param ptr address of allocated memory block or a null pointer.
     */      
    void Allocator::free(void* ptr)
    {
        if(heap_ != NULL && ptr != NULL) 
        {
            heap_->free(ptr);  
        }
    }
    
    /**
     * Sets some heap memory.
     *
     * @param heap a constructed heap object.
     */
    void Allocator::setHeap(::api::Heap& heap)
    {
        heap_ = heap.isConstructed() ? &heap : NULL;
    }
      
    /**
     * Pointer to constructed heap memory (no boot).
     */
    ::api::Heap* Allocator::heap_;

}
