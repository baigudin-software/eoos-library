/**
 * @file      lib.SmartPointerDeleter.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2022-2024, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_SMARTPOINTERDELETER_HPP_
#define LIB_SMARTPOINTERDELETER_HPP_

#include "lib.Types.hpp"

namespace eoos
{
namespace lib
{

/**
 * @class SmartPointerDeleter<T>
 * @brief Deleter of smart pointers allocated with new operator.
 *
 * @tparam T Data type of an owning object. 
 */
template <typename T>
class SmartPointerDeleter
{
    
public:

    /**
     * @brief Frees an allocated object.
     *
     * @param ptr Address of allocated the owning object.
     */
    static void free(T* const ptr);

};

/**
 * @class SmartPointerDeleterArray<T>
 * @brief Deleter of smart pointers allocated with new [] operator.
 *
 * @tparam T Data type of an owning object. 
 */
template <typename T>
class SmartPointerDeleterArray
{
    
public:

    /**
     * @brief Frees an allocated array of objects.
     *
     * @param ptr Address of allocated the owning objects.
     */
    static void free(T* const ptr);

};

template <typename T>
void SmartPointerDeleter<T>::free(T* const ptr)
{
    delete ptr;
}

template <typename T>
void SmartPointerDeleterArray<T>::free(T* const ptr)
{
    delete [] ptr;
}

} // namespace lib
} // namespace eoos
#endif // LIB_SMARTPOINTERDELETER_HPP_
