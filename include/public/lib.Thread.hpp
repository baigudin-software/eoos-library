/**
 * @brief Thread.
 *
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
 * @brief Thread class.
 *
 * @tparam A heap memory allocator class. 
 */
template <class A = Allocator>   
class Thread : public Object<A>, public api::Thread
{
    typedef Thread Self;
    typedef Object<A> Parent;

public:

    /**
     * @brief Constructor.
     *
     * @param task - a task interface whose start function is invoked when this thread is started.
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
     * @brief Tests if this object has been constructed.
     *
     * @return true if object has been constructed successfully.
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }

    /**
     * @brief Causes this thread to begin execution.
     */
    virtual void execute()
    {
        if( Self::isConstructed() )
        {
            thread_->execute();
        }
    }

    /**
     * @brief Waits for this thread to die.
     */
    virtual void join()
    {
        if( Self::isConstructed() )
        {
            thread_->join();
        }
    }

    /**
     * @brief Returns the identifier of this thread.
     *
     * @return the thread identifier.
     */
    virtual int64_t getId() const
    {
        int64_t id = WRONG_ID;
        if( Self::isConstructed() )
        {
            id = thread_->getId();
        }
        return id;
    }

    /**
     * @brief Returns a status of this thread.
     *
     * @return this thread status.
     */
    virtual api::Thread::Status getStatus() const
    {
        api::Thread::Status status = DEAD;
        if( Self::isConstructed() )
        {
            status = thread_->getStatus();
        }
        return status;
    }

    /**
     * @brief Returns this thread priority.
     *
     * @return priority value.
     */
    virtual int32_t getPriority() const
    {
        int32_t priority = -1;
        if( Self::isConstructed() )
        {
            priority = thread_->getPriority();
        }
        return priority;
    }

    /**
     * @brief Sets this thread priority.
     *
     * @param priority - number of priority in range [MIN_PRIORITY, MAX_PRIORITY], or LOCK_PRIORITY.
     */
    virtual void setPriority(int32_t const priority)
    {
        if( Self::isConstructed() )
        {
            thread_->setPriority(priority);
        }
    }
    
    /**
     * @brief Returns an error of this thread task execution.
     *
     * @return an execution error.
     */
    virtual int32_t getExecutionError() const
    {
        int32_t error {-1};
        if( Self::isConstructed() )
        {
            error = thread_->getExecutionError();
        }
        return error;
    }

    /**
     * @brief Returns currently executing thread.
     *
     * @return executing thread.
     */
    static api::Thread& getCurrent()
    {
        return getScheduler().getCurrentThread();
    }

    /**
     * @brief Causes current thread to sleep.
     *
     * @param millis - a time to sleep in milliseconds.
     * @param nanos  - an additional nanoseconds to sleep.
     */
    static void sleep(int64_t const millis, int32_t const nanos = 0)
    {
        getScheduler().sleepCurrentThread(millis, nanos);
    }

    /**
     * @brief Yields to next thread.
     */
    static void yield()
    {
        getScheduler().yield();
    }

    /**
     * @brief Returns the toggle interface for controlling global thread switching.
     *
     * @return toggle interface.
     */
    static api::Toggle& toggle()
    {
        return getScheduler().toggle();
    }

private:

    /**
     * @brief Constructor.
     *
     * @param task - an task interface whose main method is invoked when this thread is started.
     * @return true if object has been constructed successfully.
     */
    bool_t construct(api::Task& task)
    {
        bool_t res = Self::isConstructed();
        if( res == true )
        {
            thread_ = getScheduler().createThread(task);
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
        static api::Scheduler& scheduler = sys::Call::get().getScheduler();
        return scheduler;
    }
    

    /**
     * @brief Copy constructor.
     *
     * @param obj - reference to source object.
     */
    Thread(const Thread& obj);

    /**
     * @brief Assignment operator.
     *
     * @param obj - reference to source object.
     * @return reference to this object.
     */
    Thread& operator=(const Thread& obj);
    
    #if EOOS_CPP_STANDARD >= 2011

    /**
     * @brief Move constructor.
     *
     * @param obj Right reference to a source object.     
     */       
    Thread(Thread&& obj) noexcept = delete; 
    
    /**
     * @brief Move assignment operator.
     *
     * @param obj Right reference to a source object.
     * @return reference to this object.
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
