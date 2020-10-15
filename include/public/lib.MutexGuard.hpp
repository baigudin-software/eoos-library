/**
 * Guard of mutex.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_MUTEX_GUARD_HPP_
#define LIB_MUTEX_GUARD_HPP_

#include "lib.Object.hpp"
#include "api.Mutex.hpp"

namespace eoos
{
    namespace lib
    {
        template <class A = Allocator>
        class MutexGuard : public Object<A>
        {
            typedef ::eoos::lib::Object<A> Parent;            
        
        public:

            /**
             * Constructor.
             *
             * @param mutex - a mutex for guarding.
             */
            explicit MutexGuard(api::Mutex& mutex) : Parent(),
                mutex_ (mutex){
                bool_t const isConstructed = construct();
                setConstructed( isConstructed );                    
            }
            
            /**        
             * Destructor.
             */
            virtual ~MutexGuard() noexcept
            {
                if( isConstructed() )
                {
                    mutex_.unlock();
                }
            }
            
            /**
             * Tests if this object has been constructed.
             *
             * @return true if object has been constructed successfully.
             */
            virtual bool_t isConstructed() const
            {
                return Parent::isConstructed();
            }            
                
        private:
        
            /**        
             * Constructs this object.
             *
             * @returns true if this object has been constructed successfully.
             */
            bool construct()
            {
                bool res = false;
                do
                {
                    if( not isConstructed() )
                    {
                        break;
                    }
                    if( not mutex_.isConstructed() )
                    {
                        break;
                    }
                    res = mutex_.lock();
                } while(false);
                return res;
            }
            
            /**        
             * Copy constructor.
             */
            MutexGuard(const MutexGuard&);
        
            /**        
             * Copy assignment operator.
             */
            MutexGuard& operator=(const MutexGuard&);
        
            /**
             * Mutex resource identifier.
             */
            api::Mutex& mutex_;
        };


    } // namespace exec
} // namespace ara

#endif // LIB_MUTEX_GUARD_HPP_
