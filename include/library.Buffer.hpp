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

namespace global
{
    namespace library
    { 
        /** 
         * Primary template implements the static buffer class.
         *
         * @param T data type of buffer element.     
         * @param L maximum number of buffer elements, or 0 for dynamic allocation. 
         * @param A heap memory allocator class.
         */ 
        template <typename T, int32 L, class A = Allocator>
        class Buffer : public library::AbstractBuffer<T,A>
        {
            typedef library::AbstractBuffer<T,A> Parent;
    
        public:      
    
            /** 
             * Constructor.
             */    
            Buffer() : Parent(L),
                buf_ (arr_){
            }
        
            /** 
             * Constructor.
             *
             * @param illegal illegal value.
             */    
            Buffer(const T illegal) : Parent(L, illegal),
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
            Buffer& operator =(const Buffer<T,L,A>& buf)
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
            Buffer& operator =(const AbstractBuffer<T,A>& buf)
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
            virtual T* getBuffer() const
            {
                if( not this->isConstructed_ ) 
                {
                    return this->NULL;
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
             * Current array of T elements.
             */  
            T arr_[L];
            
            /**
             * Pointer to current array.
             */    
            T* buf_;
        
        };
        
        #ifdef NO_STRICT_MISRA_RULES
    
        /** 
         * Partial specialization of the template implements the dynamic buffer class.
         *
         * @param T data type of buffer element.     
         * @param A heap memory allocator class.
         */
        template <typename T, class A>
        class Buffer<T,0,A> : public AbstractBuffer<T,A>
        {
            typedef library::AbstractBuffer<T,A> Parent;
    
        public:      
    
            /** 
             * Constructor.
             *
             * @param count count of buffer elements.
             */    
            Buffer(int32 count) : Parent(count),
                buf_      (this->NULL),
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
            Buffer(int32 count, T* buf) : Parent (count),
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
            Buffer(int32 count, const T illegal) : Parent(count, illegal),
                buf_      (this->NULL),
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
            Buffer(int32 count, const T illegal, T* buf) : Parent(count, illegal),
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
            Buffer& operator =(const Buffer<T,0,A>& buf)
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
            Buffer& operator =(const AbstractBuffer<T,A>& buf)
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
            virtual T* getBuffer() const
            {
                if( not this->isConstructed_ )
                {
                    return this->NULL;
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
                if(buf_ == this->NULL) 
                {
                    buf_ = this->template allocate<T*>(count * sizeof(T));
                }
                return buf_ == this->NULL ? false : true;
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
            T* buf_;
            
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
}
#endif // LIBRARY_BUFFER_HPP_
