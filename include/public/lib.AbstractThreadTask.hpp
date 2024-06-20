/**
 * @file      lib.AbstractThreadTask.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2022-2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ABSTRACTTHREADTASK_HPP_
#define LIB_ABSTRACTTHREADTASK_HPP_

#include "lib.Thread.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class AbstractThreadTask<A>
 * @brief Abstract thread with a task.
 *
 * @tparam A Heap memory allocator class. 
 */
template <class A = Allocator>   
class AbstractThreadTask : public Thread<A>, public api::Task
{
    typedef Thread<A> Parent;

public:

    /**
     * @brief Constructor.
     */
    AbstractThreadTask();

    /**
     * @brief Destructor.
     */
    virtual ~AbstractThreadTask();

    /**
     * @copydoc eoos::api::Task::start()
     */        
    virtual void start() = 0;

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const;

    /**
     * @copydoc eoos::api::Task::getStackSize()
     */
    virtual size_t getStackSize() const;

protected:

    using Parent::setConstructed;
    using Parent::setTask;

private:

    /**
     * @brief Constructor.
     *
     * @return True if object has been constructed successfully.
     */
    bool_t construct();

};

template <class A>
AbstractThreadTask<A>::AbstractThreadTask() 
    : Thread<A>()
    , api::Task() {
    bool_t const isConstructed( construct() );
    setConstructed( isConstructed );        
}

template <class A>
AbstractThreadTask<A>::~AbstractThreadTask()
{
}

template <class A>
bool_t AbstractThreadTask<A>::isConstructed() const
{
    return Parent::isConstructed();
}

template <class A>
size_t AbstractThreadTask<A>::getStackSize() const
{
    return 0U;
}

template <class A>
bool_t AbstractThreadTask<A>::construct()
{
    return setTask(*this);
}

} // namespace lib
} // namespace eoos
#endif // LIB_ABSTRACTTHREADTASK_HPP_
