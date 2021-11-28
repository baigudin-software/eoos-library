/**
 * @file      lib.Types.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020-2021, Sergey Baigudin, Baigudin Software
 *
 * @brief Library type definitions.
 */
#ifndef LIB_TYPES_HPP_
#define LIB_TYPES_HPP_

#include "Types.hpp"

namespace eoos
{
namespace lib
{
    
#if EOOS_CPP_STANDARD >= 2011

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

/**
 * @brief Empty class type used to disambiguate the overloads of throwing and non-throwing allocation.
 */
struct nothrow_t 
{ 
    explicit nothrow_t() = default; 
};

/**
 * @brief Constant of type lib::nothrow_t used to disambiguate the overloads of throwing and non-throwing allocation functions.
 */
extern nothrow_t const nothrow;

#endif // EOOS_CPP_STANDARD >= 2011

} // namespace lib
} // namespace eoos
#endif // LIB_TYPES_HPP_
