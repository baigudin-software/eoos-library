/**
 * @file      lib.AbstractTask.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2022-2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ABSTRACTTASK_HPP_
#define LIB_ABSTRACTTASK_HPP_

#include "lib.NonCopyable.hpp"
#include "api.Task.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class AbstractTask<A>
 * @brief Abstract task for the thread class.
 *
 * @tparam A Heap memory allocator class. 
 */
template <class A = Allocator>
class AbstractTask : public NonCopyable<A>, public api::Task
{
    typedef NonCopyable<A> Parent;

public:

    /**
     * @brief Constructor.
     */
    AbstractTask();

    /**
     * @brief Destructor.
     */
    virtual ~AbstractTask();

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

};

template <class A>
AbstractTask<A>::AbstractTask() 
    : NonCopyable<A>()
    , api::Task() {
}

template <class A>
AbstractTask<A>::~AbstractTask() 
{
}

template <class A>
bool_t AbstractTask<A>::isConstructed() const
{
    return Parent::isConstructed();
}

template <class A>
size_t AbstractTask<A>::getStackSize() const
{
    return 0U;
}

} // namespace lib
} // namespace eoos
#endif // LIB_ABSTRACTTASK_HPP_
