/**
 * @file      lib.Toggle.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_TOGGLE_HPP_
#define LIB_TOGGLE_HPP_

#include "lib.NonCopyable.hpp"
#include "api.Toggle.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class Toggle<A>
 * @brief Toggle of toggle interface.
 *
 * @tparam A Heap memory allocator class.
 */
template <class A = Allocator>
class Toggle : public NonCopyable<A>, public api::Toggle
{
    typedef NonCopyable<A> Parent;

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
     * @param sw Reference to toggle interface for controlling its.
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
     * @param sw Reference to pointer of toggle interface for controlling its.
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
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const
    {
        bool_t isConstructed = Parent::isConstructed();
        if( isConstructed )
        {
            isConstructed = (*toggle_ == NULLPTR) ? false : true;
        }
        return isConstructed;
    }

    /**
     * @copydoc eoos::api::Toggle::disable()
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
     * @copydoc eoos::api::Toggle::enable(bool_t)
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
