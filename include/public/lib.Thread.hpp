/**
 * @file      lib.Thread.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2021, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_THREAD_HPP_
#define LIB_THREAD_HPP_

#include "lib.Object.hpp"
#include "api.Thread.hpp"
#include "sys.Call.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class Thread<A>
 * @brief Thread class.
 *
 * @tparam A Heap memory allocator class. 
 */
template <class A = Allocator>   
class Thread : public Object<A>, public api::Thread
{
    typedef Thread Self;
    typedef lib::Object<A> Parent;

public:

    /**
     * @brief Constructor.
     *
     * @param task A task interface whose start function is invoked when this thread is started.
     */
    explicit Thread(api::Task& task) : Parent(),
        thread_    (NULLPTR){
        bool_t const isConstructed = construct(task);
        setConstructed( isConstructed );
    }

    /**
     * @brief Destructor.
     */
    virtual ~Thread()
    {
        delete thread_;
    }

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }

    /**
     * @copydoc eoos::api::Thread::execute()
     */
    virtual bool_t execute()
    {
        bool_t res = false;
        if( Self::isConstructed() )
        {
            res = thread_->execute();
        }
        return res;
    }

    /**
     * @copydoc eoos::api::Thread::join()
     */
    virtual bool_t join()
    {
        bool_t res = false;
        if( Self::isConstructed() )
        {
            res = thread_->join();
        }
        return res;
    }

    /**
     * @copydoc eoos::api::Thread::getPriority()
     */
    virtual int32_t getPriority() const
    {
        int32_t priority = PRIORITY_WRONG;
        if( Self::isConstructed() )
        {
            priority = thread_->getPriority();
        }
        return priority;
    }

    /**
     * @copydoc eoos::api::Thread::setPriority(int32_t)
     */
    virtual bool_t setPriority(int32_t const priority)
    {
        return ( Self::isConstructed() ) ? thread_->setPriority(priority) : false;
    }
    
    /**
     * @copydoc eoos::api::Scheduler::sleep(int32_t)
     */
    static bool_t sleep(int32_t const ms)
    {
        return getScheduler().sleep(ms);
    }

    /**
     * @copydoc eoos::api::Scheduler::yield()
     */
    static void yield()
    {
        getScheduler().yield();
    }

protected:

    using Parent::setConstructed;      

private:

    /**
     * @brief Constructor.
     *
     * @param task An task interface whose main method is invoked when this thread is started.
     * @return True if object has been constructed successfully.
     */
    bool_t construct(api::Task& task)
    {
        bool_t res = Self::isConstructed();
        if( res == true )
        {
            api::Scheduler& scheduler = getScheduler();
            thread_ = scheduler.createThread(task);
            if(thread_ == NULLPTR || not thread_->isConstructed() )
            {
                res = false;
            }
        }
        return res;
    }
    
    /**
     * @brief Returns the OS scheduler.
     *
     * @return The OS scheduler.
     */    
    static api::Scheduler& getScheduler()
    {
        // @note Visual Studio 16 2019. The CXX compiler identification is MSVC 19.28.29335.0
        // Saving result of sys::Call::get().getScheduler() to local static variable is a cause of error
        // SEH exception with code 0xc0000005 thrown in the test body.
        // Exception thrown: read access violation. scheduler. was 0xFFFFFFFFFFFFFFEF.
        // Therefore, each time of calling this getScheduler() return the system call getScheduler() function result
        return sys::Call::get().getScheduler();
    }

    /**
     * @copydoc eoos::Object::Object(const Object&)
     */
    Thread(const Thread& obj);

    /**
     * @copydoc eoos::Object::operator=(const Object&)
     */
    Thread& operator=(const Thread& obj);
    
    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @copydoc eoos::Object::Object(const Object&&)
     */       
    Thread(Thread&& obj) noexcept = delete; 
    
    /**
     * @copydoc eoos::Object::operator=(const Object&&)
     */
    Thread& operator=(Thread&& obj) noexcept = delete;
    
    #endif // EOOS_CPP_STANDARD >= 2011

    /**
     * @brief A system scheduler thread.
     */
    api::Thread* thread_;

};

} // namespace lib
} // namespace eoos
#endif // LIB_THREAD_HPP_
