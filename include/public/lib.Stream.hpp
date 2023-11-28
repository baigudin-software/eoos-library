/**
 * @file      lib.Stream.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2022-2023, Sergey Baigudin, Baigudin Software
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
     * @enum Type
     * @brief Stream type. 
     */
    enum Type
    {
        TYPE_COUT,
        TYPE_CERR
    };

    /**
     * @brief Returns system output character stream.
     *
     * @return The system output character stream.
     */
    static api::OutStream<char_t>& cout()
    {
        return sys::Call::get().getStreamManager().getCout();
    }

    /**
     * @brief Returns system error output character stream.
     *
     * @return The system error output character stream.
     */
    static api::OutStream<char_t>& cerr()
    {
        return sys::Call::get().getStreamManager().getCerr();
    }

    /**
     * @brief Sets a new output character stream.
     *
     * @param type Stream type.
     * @param stream A new stream to set.
     * @return True if the output character stream is set.
     */
    static bool_t set(Type type, api::OutStream<char_t>& stream)
    {
        bool_t res( false );
        switch(type)
        {
            case TYPE_COUT:
            {
                res = sys::Call::get().getStreamManager().setCout(stream);
                break;
            }
            case TYPE_CERR:
            {
                res = sys::Call::get().getStreamManager().setCerr(stream);
                break;
            }
            default:
            {
                res = false;
                break;
            }
        }
        return res;
    }

    /**
     * @brief Resets the output character stream to default.
     * 
     * @param type Stream type.
     */
    static void reset(Type type)
    {
        switch(type)
        {
            case TYPE_COUT:
            {
                sys::Call::get().getStreamManager().resetCout();
                break;
            }
            case TYPE_CERR:
            {
                sys::Call::get().getStreamManager().resetCerr();
                break;
            }
            default:
            {
                break;
            }
        }
    }

};

} // namespace lib
} // namespace eoos
#endif // LIB_STREAM_HPP_
