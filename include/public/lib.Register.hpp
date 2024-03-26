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
 * @note There is R type declaration requarements:
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
 *         Value dbf   : 1;
 *         Value       : 15;
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
    Register(R& reg)
        : origin_( reg )
        , copy_( reg.value ){
    }

    /**
     * @brief Register.
     */
    ~Register() {}    

    /**
     * @brief Returns bit-field of the register.
     *
     * @return Bit-field structure of the register.
     */
    typename R::Bit& bit()
    {
       return copy_.bit;
    }

    /**
     * @brief Returns bit-field of the register.
     *
     * @return Bit-field structure of the register.
     */
    const typename R::Bit& bit() const
    {
       return copy_.bit;
    }

    /**
     * @brief Returns value of the register.
     *
     * @return Value of the register.
     */
    typename R::Value& value()
    {
       return copy_.value; 
    }

    /**
     * @brief Returns value of the register.
     *
     * @return Value of the register.
     */
    const typename R::Value& value() const
    {
       return copy_.value; 
    }

    /**
     * @brief Saves the work copy to the register.
     */
    void commit()
    {
        origin_.value = copy_.value;
    }

    /**
     * @brief Saves the register to the work copy.
     *
     * @return This register object.
     */
    Register<R>& fetch()
    {
        copy_.value = origin_.value;
        return *this;
    }
    
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

} // namespace lib
} // namespace eoos
#endif // LIB_REGISTER_HPP_
