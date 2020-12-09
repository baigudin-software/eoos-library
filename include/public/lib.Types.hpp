/**
 * @brief Library type definitions.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_TYPES_HPP_
#define LIB_TYPES_HPP_

#include "Types.hpp"

namespace eoos
{
namespace lib
{

/**
 * @brief Structs to remove references.
 */
template <class T> struct remove_reference      {typedef T type;};
template <class T> struct remove_reference<T&>  {typedef T type;};
template <class T> struct remove_reference<T&&> {typedef T type;};

/**
 * @brief Casts an object to right reference.
 */
template <class T>
constexpr typename remove_reference<T>::type&& move(T&& arg) noexcept
{
    return static_cast<typename remove_reference<T>::type&&>(arg);
}

} // namespace lib
} // namespace eoos
#endif // LIB_TYPES_HPP_
