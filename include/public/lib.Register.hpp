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
 *     typedef uint32_t reg_t;
 *     YourRegister(){}
 *     YourRegister(reg_t v){value = v;}
 *    ~YourRegister(){}
 *
 *     reg_t value;
 *     struct bit_t
 *     {
 *         reg_t inrq  : 1;
 *         reg_t sleep : 1;
 *         reg_t txfp  : 1;
 *         reg_t rflm  : 1;
 *         reg_t nart  : 1;
 *         reg_t awum  : 1;
 *         reg_t abom  : 1;
 *         reg_t ttcm  : 1;
 *         reg_t       : 7;
 *         reg_t reset : 1;
 *         reg_t       : 15;
 *         reg_t dbf   : 1;
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
    typename R::bit_t& bit();

    /**
     * @brief Returns bit-field of the register.
     *
     * @return Bit-field structure of the register.
     */
    const typename R::bit_t& bit() const;

    /**
     * @brief Returns value of the register.
     *
     * @return Value of the register.
     */
    typename R::reg_t& value();

    /**
     * @brief Returns value of the register.
     *
     * @return Value of the register.
     */
    const typename R::reg_t& value() const;

    /**
     * @brief Sets a bit value.
     *
     * @param digit Digit of a setting bit.
     * @return This register object.
     */
    Register<R>& setBit(typename R::reg_t digit);

    /**
     * @brief Cleans a bit value.
     *
     * @param digit Digit of a cleaning bit.
     * @return This register object.
     */
    Register<R>& clearBit(typename R::reg_t digit);

    /**
     * @brief Returns a bit value.
     *
     * @param digit Digit of a getting bit.
     * @return Value of the bit.
     *
     * @todo Implement const function.
     */
    typename R::reg_t getBit(typename R::reg_t digit);

    /**
     * @brief Sets a bit field value.
     *
     * @param value A value to set started from LSB.
     * @param mask  A mask of the value significat bits.
     * @param shift Shift bits for the value to set.
     * @return This register object.
     */
    Register<R>& setBitField(typename R::reg_t value, typename R::reg_t mask, typename R::reg_t shift);

    /**
     * @brief Returns a bit field value.
     *
     * @param mask  A mask of a value significat bits.
     * @param shift Shift bits for the value to get.
     * @return A value shifted to LSB.
     *
     * @todo Implement const funstion.
     */
    typename R::reg_t getBitField(typename R::reg_t mask, typename R::reg_t shift);

    /**
     * @brief Returns a bit field value.
     *
     * @param mask  A mask of a value significat bits.
     * @param shift Shift bits for the value to clear.
     * @return This register object.
     */
    Register<R>& clearBitField(typename R::reg_t mask, typename R::reg_t shift);

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
typename R::bit_t& Register<R>::bit()
{
   return copy_.bit;
}

template <class R>
const typename R::bit_t& Register<R>::bit() const
{
   return copy_.bit;
}

template <class R>
typename R::reg_t& Register<R>::value()
{
   return copy_.value;
}

template <class R>
const typename R::reg_t& Register<R>::value() const
{
   return copy_.value;
}

template <class R>
Register<R>& Register<R>::setBit(typename R::reg_t digit)
{
    typename R::reg_t bitmask( 0x00000001 << digit );
    copy_.value |= bitmask;
    return *this;
}

template <class R>
Register<R>& Register<R>::clearBit(typename R::reg_t digit)
{
    typename R::reg_t bitmask( 0x00000001 << digit );
    copy_.value &= ~bitmask;
    return *this;
}

template <class R>
typename R::reg_t Register<R>::getBit(typename R::reg_t digit)
{
    return (copy_.value >> digit) & 0x1;
}

template <class R>
Register<R>& Register<R>::setBitField(typename R::reg_t value, typename R::reg_t mask, typename R::reg_t shift)
{
    copy_.value &= ~( mask << shift);
    copy_.value |= value << shift;
    return *this;
}

template <class R>
typename R::reg_t Register<R>::getBitField(typename R::reg_t mask, typename R::reg_t shift)
{
    return (copy_.value >> shift) & mask;
}

template <class R>
Register<R>& Register<R>::clearBitField(typename R::reg_t mask, typename R::reg_t shift)
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
