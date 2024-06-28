/**
 * @file      lib.String.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020-2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_STRING_HPP_
#define LIB_STRING_HPP_

#include "lib.BaseString.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class String
 * @brief String type for chars with default allocator.
 *
 * The dynamic string for any numbers of characters with the default allocator.
 */
typedef BaseString<char_t,0> String;

} // namespace lib
} // namespace eoos
#endif // LIB_BASE_STRING_HPP_
