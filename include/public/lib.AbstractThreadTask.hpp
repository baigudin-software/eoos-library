/**
 * @file      lib.AbstractThreadTask.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2022, Sergey Baigudin, Baigudin Software
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
    AbstractThreadTask() 
        : Thread<A>()
        , api::Task() {
        bool_t const isConstructed = construct();
        setConstructed( isConstructed );        
    }

    /**
     * @brief Destructor.
     */
    virtual ~AbstractThreadTask() {}

    /**
     * @copydoc eoos::api::Task::start()
     */        
    virtual void start() = 0;

    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }

    /**
     * @copydoc eoos::api::Task::getStackSize()
     */
    virtual size_t getStackSize() const
    {
        return 0U;
    }

protected:

    using Parent::setConstructed;

private:

    /**
     * @brief Constructor.
     *
     * @return True if object has been constructed successfully.
     */
    bool_t construct()
    {
        return this->setTask(*this);
    }

};

} // namespace lib
} // namespace eoos
#endif // LIB_ABSTRACTTHREADTASK_HPP_
