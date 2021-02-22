/**
 * @brief Toggle of toggle interface.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_TOGGLE_HPP_
#define LIB_TOGGLE_HPP_

#include "lib.Object.hpp"
#include "api.Toggle.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @brief Primary template implementation.
 *
 * @tparam A heap memory allocator class.
 */
template <class A = Allocator>
class Toggle : public Object<A>, public api::Toggle
{
    typedef ::eoos::lib::Object<A> Parent;

public:

    /**
     * @brief Constructor.
     *
     * Default constructor is used for creating
     * the object which will switch nothing.
     */
    Toggle() : Parent(),
        pointer_ (NULLPTR),
        toggle_  (&pointer_){
    }

    /**
     * @brief Constructor.
     *
     * @param sw reference to toggle interface for controlling its.
     */
    Toggle(api::Toggle& sw) : Parent(),
        pointer_ (&sw),
        toggle_  (&pointer_){
    }

    /**
     * @brief Constructor.
     *
     * Until the referenced pointer equals NULLPTR the Toggle will not switch
     * given controller. This feature is the most important when a controller
     * might be used before it is initialized.
     *
     * @param sw reference to pointer of toggle interface for controlling its.
     */
    Toggle(api::Toggle*& sw) : Parent(),
        pointer_ (NULLPTR),
        toggle_  (&sw){
    }

    /**
     * @brief Destructor.
     */
    virtual ~Toggle(){}

    /**
     * @brief Tests if this object has been constructed.
     *
     * @return true if object has been constructed successfully.
     */
    virtual bool_t isConstructed() const
    {
        if( not this->isConstructed_ )
        {
            return false;
        }
        return *toggle_ == NULLPTR ? false : true;
    }

    /**
     * @brief Disables a controller.
     *
     * @return an enable source bit value of a controller before method was called.
     */
    virtual bool_t disable()
    {
        if( not isConstructed())
        {
            return false;
        }
        api::Toggle* const switcher = *toggle_;
        return switcher->disable();
    }

    /**
     * @brief Enables a controller.
     *
     * @param status returned status by disable method.
     */
    virtual void enable(const bool_t status)
    {
        if( not isConstructed() )
        {
            return;
        }
        api::Toggle* const switcher = *toggle_;
        switcher->enable(status);
    }

private:

    /**
     * @brief Copy constructor.
     *
     * @param obj reference to source object.
     */
    Toggle(const Toggle& obj);

    /**
     * @brief Assignment operator.
     *
     * @param obj reference to source object.
     */
    Toggle& operator=(const Toggle& obj);

    /**
     * @brief Pointer to always existent interface.
     */
    api::Toggle* pointer_;

    /**
     * @brief Pointer to pointer to maybe existent interface.
     */
    api::Toggle** toggle_;

};

} // namespace lib
} // namespace eoos
#endif // LIB_TOGGLE_HPP_
