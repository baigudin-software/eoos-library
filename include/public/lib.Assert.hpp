/**
 * @file      lib.Assert.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2025, Sergey Baigudin, Baigudin Software
 *
 * @brief Assert definition.
 */
#ifndef LIB_ASSERT_HPP_
#define LIB_ASSERT_HPP_

#include "Types.hpp"

namespace eoos
{
namespace lib
{

/**
 * @brief Assert.
 */
#ifdef EOOS_DEBUG_MODE
#define EOOS_ASSERT( condition )    \
{                                   \
    if( condition == false )        \
    {                               \
        while( true ){}             \
    }                               \
}
#else
#define EOOS_ASSERT( condition ) ((void)0)
#endif // EOOS_DEBUG_MODE

} // namespace lib
} // namespace eoos
#endif // LIB_TYPES_HPP_
