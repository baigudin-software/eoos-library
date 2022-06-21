/**
 * @file      lib.Stream.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_STREAM_HPP_
#define LIB_STREAM_HPP_

#include "sys.Call.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class Stream
 * @brief Stream class.
 */
class Stream
{

public:

    /**
     * @brief Returns system output character stream.
     *
     * @return The system output character stream.
     */
    static api::OutStream<char_t>& cout()
    {
        return sys::Call::get().getOutStreamChar();
    }

    /**
     * @brief Returns system error output character stream.
     *
     * @return The system error output character stream.
     */
    static api::OutStream<char_t>& cerr()
    {
        return sys::Call::get().getErrorStreamChar();
    }

};

} // namespace lib
} // namespace eoos
#endif // LIB_STREAM_HPP_
