/**
 * Stack.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_STACK_HPP_
#define LIBRARY_STACK_HPP_

#include "library.Object.hpp"
#include "api.Stack.hpp"
#include "library.Buffer.hpp"

namespace local
{
    namespace library
    {
        /** 
         * Primary template implementation.
         *
         * @param T data type of default stack element.
         * @param A heap memory allocator class.
         */ 
        template <typename T, class A = Allocator>
        class Stack : public library::Object<A>, public api::Stack<T>
        { 
            typedef library::Object<A>  Parent;  
            typedef api::Stack<T>       StackIntf;
        
        public:
        
            /** 
             * Constructor.
             *
             * @param type  type of this stack.
             * @param count count of buffer elements.
             */    
            Stack(typename api::Stack<T>::Operation type, int32 count) : Parent(),
                stack_ (count),
                type_  (type){
                const bool isConstructed = construct();
                this->setConstruct( isConstructed );
            }     
            
            /** 
             * Constructor.
             *
             * @param type  type of this stack.     
             * @param count   count of buffer elements.
             * @param illegal illegal value.
             */    
            Stack(typename api::Stack<T>::Operation type, int32 count, const T illegal) : Parent(),
                stack_ (count, illegal),
                type_  (type){
                const bool isConstructed = construct();
                this->setConstruct( isConstructed );
            }    
            
            /** 
             * Destructor.
             */    
            virtual ~Stack()
            {
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
             * Returns an initial top of stack.
             *
             * @return pointer to TOS.
             */    
            virtual const T* getTos()
            {
                if( not this->isConstructed_ ) 
                {
                    return NULL;
                }
                T* const stack = &stack_[0];
                switch(type_)
                {
                    case StackIntf::FD: 
                        return &stack[stack_.getLength()];
                        
                    case StackIntf::ED: 
                        return &stack[stack_.getLength() - 1];
                        
                    case StackIntf::FA: 
                        return &stack[0] - 1;
                        
                    case StackIntf::EA: 
                        return &stack[0];
                        
                    default: 
                        return NULL;  
                }
            }
            
            /** 
             * Returns an type of stack operation.
             *
             * @return the stack operation.
             */    
            virtual typename api::Stack<T>::Operation getType() const
            {
                return type_;
            }
            
            /**
             * Returns a number of elements.
             *
             * @return number of elements.
             */
            virtual int32 getLength() const
            {
                return stack_.getLength();
            }
        
            /**
             * Tests if this collection has elements.
             *
             * @return true if this collection does not contain any elements.
             */
            virtual bool isEmpty() const
            {
                return false;
            }
        
            /**
             * Returns illegal element which will be returned as error value.
             *
             * If illegal value is not set method returns uninitialized variable.
             *
             * @return reference to illegal element.
             */
            virtual const T& getIllegal() const
            {
                return stack_.getIllegal();
            }
        
            /**
             * Sets illegal element which will be returned as error value.
             *
             * @param value illegal value.
             */
            virtual void setIllegal(const T& value)
            {
                return stack_.setIllegal(value);
            }
        
            /**
             * Tests if given value is an illegal.
             *
             * @param value testing value.
             * @param true if value is an illegal.
             */
            virtual bool isIllegal(const T& value) const
            {
                return stack_.isIllegal(value);
            }
        
        private:
    
            /**
             * Constructor.
             *
             * @return true if object has been constructed successfully.     
             */ 
            bool construct()
            {
                if( not this->isConstructed_ ) 
                {
                    return false;
                }
                if( not stack_.isConstructed() ) 
                {
                    return false;
                }
                #ifdef EOOS_DEBUG
                stack_.fill(0);
                #endif
                return true;
            }
        
            /**
             * Copy constructor.
             *
             * @param obj reference to source object.
             */
            Stack(const Stack& obj);
        
            /**
             * Assignment operator.
             *
             * @param obj reference to source object.
             * @return reference to this object.     
             */
            Stack& operator =(const Stack& obj);
        
            /**
             * Stack memory buffer.
             */
            Buffer<T,0,A> stack_;
        
            /**
             * Stack type.
             */    
            const typename api::Stack<T>::Operation type_;
    
        };
    }
}
#endif // LIBRARY_STACK_HPP_
