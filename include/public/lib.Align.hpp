/**
 * @file      lib.Align.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2022, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ALIGN_HPP_
#define LIB_ALIGN_HPP_

#include "ObjectAllocator.hpp"

namespace eoos
{
namespace lib
{
    
/**
 * @class Align<T,S,A>
 * @brief Alignment of simple types to byte boundary of memory.
 * 
 * MISRA C++ NOTE: Any signed underlying types shall not be used
 * for not violating the 5-0-21 MISRA C++:2008 Rule.
 *
 * @tparam T Type of aligning data.
 * @tparam S Size of aligning data type.
 * @tparam A Heap memory allocator class.
 */
template <typename T, size_t S = sizeof(T), class A = Allocator>
class Align : public ObjectAllocator<A>
{

public:

    /**
     * @brief Constructor.
     */
    Align() 
        : ObjectAllocator<A>() {
    }

    /**
     * @brief Constructor.
     *
     * @note A passed value is copied to an internal data structure
     * so that the value might be invalidated after the function called.
     *
     * @param value A data value.
     */
    Align(T const& value) 
        : ObjectAllocator<A>() {
        assignment(value);
    }

    /**
     * @brief Copy constructor.
     *
     * @param obj A source object.
     */
    Align(Align const& obj) ///< SCA MISRA-C++:2008 Justified Rule 12-8-1
        : ObjectAllocator<A>(obj) {
        copy(obj);
    }

    /**
     * @brief Assignment operator.
     *
     * @note A passed value is copied to an internal data structure
     * so that the value might be invalidated after the function called.
     *
     * @param value A source data value.
     * @return Reference to this object.
     */
    Align& operator=(T const& value)
    {
        assignment(value);
        return *this;
    }

    /**
     * @brief Assignment operator.
     *
     * @param obj A source object.
     * @return Reference to this object.
     */
    Align& operator=(Align const& obj)
    {
        copy(obj);
        return *this;
    }

    /**
     * @brief Pre-increment operator.
     *
     * @param obj A source object.
     * @return Reference to this object.
     */
    Align& operator++()
    {
        T val( typecast() );
        val += 1;
        assignment(val);
        return *this;
    }

    /**
     * @brief Pre-decrement operator.
     *
     * @param obj A source object.
     * @return Reference to this object.
     */
    Align& operator--()
    {
        T val( typecast() );
        val -= 1;
        assignment(val);
        return *this;
    }

    /**
     * @brief Post-increment operator.
     *
     * @return This object.
     */
    Align operator++(int) ///< SCA MISRA-C++:2008 Justified Rule 3-9-2
    {
        T val( typecast() );
        val += 1;
        assignment(val);
        return *this;
    }

    /**
     * @brief Post-decrement operator.
     *
     * @return This object.
     */
    Align operator--(int) ///< SCA MISRA-C++:2008 Justified Rule 3-9-2
    {
        T val( typecast() );
        val -= 1;
        assignment(val);
        return *this;
    }

    /**
     * @brief Casts to the template data type.
     *
     * @return A data value.
     */
    operator T() const
    {
        return typecast();
    }

private:

    /**
     * @brief Compares a type based value with this class.
     *
     * @param val A source value.
     * @return True if this object value equals to a passed value.
     */
    bool_t equal(T const& value) const
    {
        Align<T,S,A> obj(value);
        return equal(obj);
    }

    /**
     * @brief Compares an object of this class type with this class.
     *
     * @param obj A source object.
     * @return True if this object value equals to a passed object value.
     */
    bool_t equal(Align const& obj) const
    {
        bool_t res( true );
        for(size_t i(0U); i<S; i++)
        {
            if( val_[i] != obj.val_[i] )
            {
                res = false;
                break;
            }
        }
        return res;
    }

    /**
     * @brief Assigns given value to self data.
     *
     * @param value Source data value.
     */
    void assignment(T const& value) ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        for(size_t i(0U); i<S; i++)
        {
            T const v( value >> (8U * i) ); ///< SCA MISRA-C++:2008 Justified Rule 5-0-21
            val_[i] = static_cast<cell_t>(v);
        }
    }

    /**
     * @brief Copies given object to self data.
     *
     * @param obj Reference to source object.
     */
    void copy(Align const& obj) ///< SCA MISRA-C++:2008 Defected Rule 9-3-3
    {
        for(size_t i(0U); i<S; i++)
        {
            val_[i] = obj.val_[i];
        }
    }

    /**
     * @brief Returns conversed data to type of aligning data.
     *
     * @return Conversed data.
     */
    T typecast() const
    {
        int32_t const max( static_cast<int32_t>(S) - 1 );
        T r( static_cast<T>(0) );
        for(int32_t i(max); i>=0; i--)
        {
            r = r << 8U;                     ///< SCA MISRA-C++:2008 Justified Rule 5-0-21
            r = r | static_cast<T>(val_[i]); ///< SCA MISRA-C++:2008 Justified Rule 5-0-21
        }
        return r;
    }

    /**
     * @brief Array of data bytes.
     */
    ucell_t val_[S];

};

/**
 * @brief Comparison operator to equal.
 *
 * @param obj1 Reference to object.
 * @param obj2 Reference to object.
 * @return True if objects are equal.
 */
template <typename T, size_t S = sizeof(T), class A = Allocator>
inline bool_t operator==(Align<T,S,A> const& obj1, Align<T,S,A> const& obj2)
{
    bool_t const res( obj1.equal(obj2) );
    return res;
}

/**
 * @brief Comparison operator to unequal.
 *
 * @param obj1 Reference to object.
 * @param obj2 Reference to object.
 * @return True if objects are not equal.
 */
template <typename T, size_t S = sizeof(T), class A = Allocator>
inline bool_t operator!=(Align<T,S,A> const& obj1, Align<T,S,A> const& obj2)
{
    bool_t const res( obj1.equal(obj2) );
    return !res;
}

} // namespace lib
} // namespace eoos
#endif // LIB_ALIGN_HPP_
