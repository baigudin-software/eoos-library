/** 
 * Buffer class in static and dynamic specializations.
 *
 * This class has a primary template and a partial specialization of the template. 
 * The non-specialized template defines a realization that contains a whole buffer, 
 * which is defined by a template argument, as data member of the class. 
 * The specialization allocates necessary memory size for containing the buffer 
 * in a heap memory.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2018, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_BUFFER_HPP_
#define LIBRARY_BUFFER_HPP_

#include "library.AbstractBuffer.hpp"

namespace library
{ 
    /** 
     * Primary template implements the static buffer class.
     *
     * @param Type      data type of buffer element.     
     * @param MAX_COUNT maximum number of buffer elements, or 0 for dynamic allocation. 
     * @param Alloc     heap memory allocator class.
     */ 
    template <typename Type, int32 MAX_COUNT, class Alloc = Allocator>
    class Buffer : public ::library::AbstractBuffer<Type,Alloc>
    {
        typedef ::library::AbstractBuffer<Type,Alloc> Parent;
  
    public:      
  
        /** 
         * Constructor.
         */    
        Buffer() : Parent(MAX_COUNT),
            buf_ (arr_){
        }
    
        /** 
         * Constructor.
         *
         * @param illegal illegal value.
         */    
        Buffer(const Type illegal) : Parent(MAX_COUNT, illegal),
            buf_ (arr_){
        }
    
        /**
         * Destructor.
         */
        virtual ~Buffer()
        {
        }
    
        /**
         * Assignment operator.
         *
         * If the source buffer is greater than this buffer,
         * only cropped data of that will be copied.
         *
         * @param buf reference to source buffer.
         * @return reference to this object.     
         */
        Buffer& operator =(const Buffer<Type,MAX_COUNT,Alloc>& buf)
        {
            this->copy(buf);
            return *this;      
        }
    
        /**
         * Assignment operator.
         *
         * If the source buffer is greater than this buffer,
         * only cropped data of that will be copied.
         *
         * @param buf reference to source buffer.
         * @return reference to this object.     
         */
        Buffer& operator =(const AbstractBuffer<Type,Alloc>& buf)
        {
            this->copy(buf);
            return *this;
        }
  
    protected: 
  
        /**
         * Returns a pointer to the fist buffer element.
         *
         * @return pointer to buffer, or NULL.
         */
        virtual Type* getBuffer() const
        {
            if( not this->isConstructed_ ) 
            {
                return NULL;
            }
            return buf_;
        }
  
    private: 
  
        /**
         * Copy constructor.
         *
         * @param obj reference to source object.
         */
        Buffer(const Buffer& obj);
      
        /**
         * Current array of Type elements.
         */  
        Type arr_[MAX_COUNT];
        
        /**
         * Pointer to current array.
         */    
        Type* buf_;
      
    };
    
    #ifdef NO_STRICT_MISRA_RULES
  
    /** 
     * Partial specialization of the template implements the dynamic buffer class.
     *
     * @param Type  data type of buffer element.     
     * @param Alloc heap memory allocator class.
     */
    template <typename Type, class Alloc>
    class Buffer<Type,0,Alloc> : public AbstractBuffer<Type,Alloc>
    {
        typedef ::library::AbstractBuffer<Type,Alloc> Parent;
  
    public:      
  
        /** 
         * Constructor.
         *
         * @param count count of buffer elements.
         */    
        Buffer(int32 count) : Parent(count),
            buf_      (NULL),
            isDelete_ (true){
            const bool isConstructed = construct(count);
            this->setConstruct( isConstructed );
        }
      
        /** 
         * Constructor.
         *
         * NOTE: Given external buffer has to exist until this object is being lived,
         * and will NOT be deleted when the buffer is being deleted.
         *
         * @param count number of elements.
         * @param buf   pointer to external buffer.
         */    
        Buffer(int32 count, Type* buf) : Parent (count),
            buf_      (buf),
            isDelete_ (false){
            const bool isConstructed = construct(count);
            this->setConstruct( isConstructed );
        }
      
        /** 
         * Constructor.
         *
         * @param count   count of buffer elements.
         * @param illegal illegal value.
         */    
        Buffer(int32 count, const Type illegal) : Parent(count, illegal),
            buf_      (NULL),
            isDelete_ (true){
            const bool isConstructed = construct(count);
            this->setConstruct( isConstructed );
        }
      
        /** 
         * Constructor.
         *
         * NOTE: Given external buffer has to exist until this object is being lived,
         * and will NOT be deleted when the buffer is being deleted.     
         *
         * @param count   number of elements.
         * @param illegal illegal value.
         * @param buf     pointer to external buffer.
         */    
        Buffer(int32 count, const Type illegal, Type* buf) : Parent(count, illegal),
            buf_      (buf),
            isDelete_ (false){
            const bool isConstructed = construct(count);
            this->setConstruct( isConstructed );
        }
      
      
        /**
         * Destructor.
         */
        virtual ~Buffer()
        {
            if(isDelete_) 
            {
                this->free(buf_);
            }
        }
      
        /**
         * Assignment operator.
         *
         * If the source buffer is greater than this buffer,
         * only cropped data of that will be copied.
         *
         * @param buf reference to source buffer.
         * @return reference to this object.     
         */
        Buffer& operator =(const Buffer<Type,0,Alloc>& buf)
        {
            this->copy(buf);
            return *this;      
        }
      
        /**
         * Assignment operator.
         *
         * If the source buffer is greater than this buffer,
         * only cropped data of that will be copied.
         *
         * @param buf reference to source buffer.
         * @return reference to this object.     
         */
        Buffer& operator =(const AbstractBuffer<Type,Alloc>& buf)
        {
            this->copy(buf);
            return *this;
        }
  
    protected: 
  
        /**
         * Returns a pointer to the fist buffer element.
         *
         * @return pointer to buffer, or NULL.
         */
        virtual Type* getBuffer() const
        {
            if( not this->isConstructed_ )
            {
                return NULL;
            }
            return buf_;
        }
  
    private: 
  
        /** 
         * Constructor.
         *
         * @param count count of buffer elements.
         * @return boolean result.
         */
        bool construct(const int32 count)
        {
            if( not this->isConstructed_ ) 
            {
                return false;
            }
            // If you have a WTF question looking at the next construction, then look
            // at description of 'allocate' template method of 'Object' template class.
            if(buf_ == NULL) 
            {
                buf_ = this->template allocate<Type*>(count * sizeof(Type));
            }
            return buf_ == NULL ? false : true;
        }
      
        /**
         * Copy constructor.
         *
         * @param obj reference to source object.
         */
        Buffer(const Buffer& obj);
      
        /**
         * Pointer to external given or self created array.
         */     
        Type* buf_;
        
        /**
         * Deletet flag.
         *
         * Is set to true for self created array or 
         * set to false for external given array.
         */        
        bool isDelete_;
      
    };
    
    #endif // NO_STRICT_MISRA_RULES
    
}
#endif // LIBRARY_BUFFER_HPP_
