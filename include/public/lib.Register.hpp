/**
 * @file      lib.Register.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_REGISTER_HPP_
#define LIB_REGISTER_HPP_

#include "lib.Types.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class Register<R>
 * @brief Alignment of fundamental types to byte boundary of memory.
 *
 * @tparam R User-defined union type of register.
 *
 * @note There is R type declaration requirements:
 * ```
 * union YourRegister
 * {
 *     typedef uint32_t Value;
 *     YourRegister(){}
 *     YourRegister(Value v){value = v;}
 *    ~YourRegister(){}
 *
 *     Value value;
 *     struct Bit
 *     {
 *         Value inrq  : 1;
 *         Value sleep : 1;
 *         Value txfp  : 1;
 *         Value rflm  : 1;
 *         Value nart  : 1;
 *         Value awum  : 1;
 *         Value abom  : 1;
 *         Value ttcm  : 1;
 *         Value       : 7;
 *         Value reset : 1;
 *         Value       : 15;
 *         Value dbf   : 1;
 *     } bit;
 * };
 * ```
 */
template <class R>
class Register
{

public:

    /**
     * @brief Constructor.
     */
    Register(R& reg);

    /**
     * @brief Desctructor.
     */
    ~Register();

    /**
     * @brief Returns bit-field of the register.
     *
     * @return Bit-field structure of the register.
     */
    typename R::Bit& bit();

    /**
     * @brief Returns bit-field of the register.
     *
     * @return Bit-field structure of the register.
     */
    const typename R::Bit& bit() const;

    /**
     * @brief Returns value of the register.
     *
     * @return Value of the register.
     */
    typename R::Value& value();

    /**
     * @brief Returns value of the register.
     *
     * @return Value of the register.
     */
    const typename R::Value& value() const;

    /**
     * @brief Sets a bit value.
     *
     * @param digit Digit of a setting bit.
     * @return This register object.
     */
    Register<R>& setBit(typename R::Value digit);

    /**
     * @brief Cleans a bit value.
     *
     * @param digit Digit of a cleaning bit.
     * @return This register object.
     */
    Register<R>& clearBit(typename R::Value digit);

    /**
     * @brief Returns a bit value.
     *
     * @param digit Digit of a getting bit.
     * @return Value of the bit.
     *
     * @todo Implement const function.
     */
    typename R::Value getBit(typename R::Value digit);

    /**
     * @brief Sets a bit field value.
     *
     * @param value A value to set started from LSB.
     * @param mask  A mask of the value significat bits.
     * @param shift Shift bits for the value to set.
     * @return This register object.
     */
    Register<R>& setBitField(typename R::Value value, typename R::Value mask, typename R::Value shift);

    /**
     * @brief Returns a bit field value.
     *
     * @param mask  A mask of a value significat bits.
     * @param shift Shift bits for the value to get.
     * @return A value shifted to LSB.
     *
     * @todo Implement const funstion.
     */
    typename R::Value getBitField(typename R::Value mask, typename R::Value shift);

    /**
     * @brief Returns a bit field value.
     *
     * @param mask  A mask of a value significat bits.
     * @param shift Shift bits for the value to clear.
     * @return This register object.
     */
    Register<R>& clearBitField(typename R::Value mask, typename R::Value shift);

    /**
     * @brief Saves the work copy to the register.
     *
     * @return This register object.
     */
    Register<R>& commit();

    /**
     * @brief Saves the register to the work copy.
     *
     * @return This register object.
     */
    Register<R>& fetch();

private:

    /**
     * @brief The register.
     */
    R& origin_;

    /**
     * @brief Work copy.
     */
    R copy_;

};

template <class R>
Register<R>::Register(R& reg)
    : origin_( reg )
    , copy_( reg.value ){
}

template <class R>
Register<R>::~Register()
{
}

template <class R>
typename R::Bit& Register<R>::bit()
{
   return copy_.bit;
}

template <class R>
const typename R::Bit& Register<R>::bit() const
{
   return copy_.bit;
}

template <class R>
typename R::Value& Register<R>::value()
{
   return copy_.value;
}

template <class R>
const typename R::Value& Register<R>::value() const
{
   return copy_.value;
}

template <class R>
Register<R>& Register<R>::setBit(typename R::Value digit)
{
    typename R::Value bitmask( 0x00000001 << digit );
    copy_.value |= bitmask;
    return *this;
}

template <class R>
Register<R>& Register<R>::clearBit(typename R::Value digit)
{
    typename R::Value bitmask( 0x00000001 << digit );
    copy_.value &= ~bitmask;
    return *this;
}

template <class R>
typename R::Value Register<R>::getBit(typename R::Value digit)
{
    return (copy_.value >> digit) & 0x1;
}

template <class R>
Register<R>& Register<R>::setBitField(typename R::Value value, typename R::Value mask, typename R::Value shift)
{
    copy_.value &= ~( mask << shift);
    copy_.value |= value << shift;
    return *this;
}

template <class R>
typename R::Value Register<R>::getBitField(typename R::Value mask, typename R::Value shift)
{
    return (copy_.value >> shift) & mask;
}

template <class R>
Register<R>& Register<R>::clearBitField(typename R::Value mask, typename R::Value shift)
{
    copy_.value &= ~( mask << shift);
    return *this;
}

template <class R>
Register<R>& Register<R>::commit()
{
    origin_.value = copy_.value;
    return *this;
}

template <class R>
Register<R>& Register<R>::fetch()
{
    copy_.value = origin_.value;
    return *this;
}

} // namespace lib
} // namespace eoos
#endif // LIB_REGISTER_HPP_
