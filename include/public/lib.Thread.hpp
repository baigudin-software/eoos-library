/**
 * @file      lib.Thread.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_THREAD_HPP_
#define LIB_THREAD_HPP_

#include "lib.NonCopyable.hpp"
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
class Thread : public NonCopyable<A>, public api::Thread
{
    typedef NonCopyable<A> Parent;

public:

    /**
     * @brief Constructor.
     *
     * @param task A task interface whose start function is invoked when this thread is started.
     */
    Thread(api::Task& task);

    /**
     * @brief Destructor.
     */
    virtual ~Thread();

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const;

    /**
     * @copydoc eoos::api::Thread::execute()
     */
    virtual bool_t execute();

    /**
     * @copydoc eoos::api::Thread::join()
     */
    virtual bool_t join();

    /**
     * @copydoc eoos::api::Thread::getPriority()
     */
    virtual int32_t getPriority() const;

    /**
     * @copydoc eoos::api::Thread::setPriority(int32_t)
     */
    virtual bool_t setPriority(int32_t const priority);
    
    /**
     * @copydoc eoos::api::Scheduler::sleep(int32_t)
     */
    static bool_t sleep(int32_t const ms);

    /**
     * @copydoc eoos::api::Scheduler::yield()
     */
    static bool_t yield();

protected:

    using Parent::setConstructed;
    
    /**
     * @brief Constructor.
     *
     * @param task A task interface whose start function is invoked when this thread is started.
     */
    Thread();
    
    /**
     * @brief Sets a task.
     *
     * @param task An task interface whose start function is invoked when this thread is started.
     * @return True if the task has been set successfully.
     */
    bool_t setTask(api::Task& task);

private:

    /**
     * @brief Constructor.
     *
     * @param task An task interface whose start function is invoked when this thread is started.
     * @return True if object has been constructed successfully.
     */
    bool_t construct(api::Task* const task);
    
    /**
     * @brief Returns the OS scheduler.
     *
     * @return The OS scheduler.
     */    
    static api::Scheduler& getScheduler();

    /**
     * @brief A system scheduler thread.
     */
    api::Thread* thread_;

};

template <class A>
Thread<A>::Thread(api::Task& task)
    : NonCopyable<A>()
    , api::Thread()
    , thread_(NULLPTR) {
    bool_t const isConstructed( construct(&task) );
    setConstructed( isConstructed );
}

template <class A>
Thread<A>::~Thread()
{
    if( thread_ != NULLPTR )
    {
        delete thread_;
    }
}

template <class A>
bool_t Thread<A>::isConstructed() const
{
    return Parent::isConstructed();
}

template <class A>
bool_t Thread<A>::execute()
{
    bool_t res( false );
    if( isConstructed() && (thread_ != NULLPTR) )
    {
        res = thread_->execute();
    }
    return res;
}

template <class A>
bool_t Thread<A>::join()
{
    bool_t res( false );
    if( isConstructed() && (thread_ != NULLPTR) )
    {
        res = thread_->join();
    }
    return res;
}

template <class A>
int32_t Thread<A>::getPriority() const
{
    int32_t priority( PRIORITY_WRONG );
    if( isConstructed() && (thread_ != NULLPTR) )
    {
        priority = thread_->getPriority();
    }
    return priority;
}

template <class A>
bool_t Thread<A>::setPriority(int32_t const priority)
{
    return ( isConstructed() && (thread_ != NULLPTR) ) ? thread_->setPriority(priority) : false;
}

template <class A>
bool_t Thread<A>::sleep(int32_t const ms)
{
    return getScheduler().sleep(ms);
}

template <class A>
bool_t Thread<A>::yield()
{
    return getScheduler().yield();
}

template <class A>
Thread<A>::Thread() 
    : NonCopyable<A>()
    , api::Thread()
    , thread_(NULLPTR){
    bool_t const isConstructed( construct(NULLPTR) );
    setConstructed( isConstructed );
}    

template <class A>
bool_t Thread<A>::setTask(api::Task& task)
{
    bool_t res( false );
    if( thread_ == NULLPTR )
    {
        api::Scheduler& scheduler( getScheduler() );
        thread_ = scheduler.createThread(task);
        if( (thread_ != NULLPTR) && thread_->isConstructed() )
        {
            res = true;
        }
    }
    return res;
}

template <class A>
bool_t Thread<A>::construct(api::Task* const task)
{
    bool_t res( isConstructed() );
    if( res == true )
    {
        if(task != NULLPTR)
        {
            res = setTask(*task);
        }
    }
    return res;
}

template <class A>
api::Scheduler& Thread<A>::getScheduler()
{
    // @note Visual Studio 16 2019. The CXX compiler identification is MSVC 19.28.29335.0
    // Saving result of sys::Call::get().getScheduler() to local static variable is a cause of error
    // SEH exception with code 0xc0000005 thrown in the test body.
    // Exception thrown: read access violation. scheduler. was 0xFFFFFFFFFFFFFFEF.
    // Therefore, each time of calling this getScheduler() return the system call getScheduler() function result
    return sys::Call::get().getScheduler();
}

} // namespace lib
} // namespace eoos
#endif // LIB_THREAD_HPP_
