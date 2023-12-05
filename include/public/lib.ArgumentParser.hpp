/**
 * @file      lib.ArgumentParser.hpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2023, Sergey Baigudin, Baigudin Software
 */
#ifndef LIB_ARGUMENTPARSER_HPP_
#define LIB_ARGUMENTPARSER_HPP_

#include "lib.NonCopyable.hpp"
#include "lib.LinkedList.hpp"
#include "lib.BaseString.hpp"

#include <iostream>

namespace eoos
{
namespace lib
{

/**
 * @class ArgumentParser<A>
 * @brief Program argument parser class.
 *
 * @tparam T A data type of argument string characters.
 * @tparam L A maximum number of argument string, or 0 for dynamic argument length.
 * @tparam A Heap memory allocator class. 
 */
template <typename T, int32_t L, class A = Allocator>
class ArgumentParser : public NonCopyable<A>
{
    typedef NonCopyable<A> Parent;

public:

    /**
     * @brief Constructor.
     * 
     * @param argc The number of arguments passed to the program.
     * @param argv An array of c-string of arguments where the last one - argc + 1 is null.
     */
    ArgumentParser(int32_t argc, T* argv[])
        : NonCopyable<A>()
        , args_ ()
        , ptrs_ (NULLPTR) {
        bool_t const isConstructed( construct(argc, argv) );
        setConstructed( isConstructed );
    }

    /**
     * @brief Destructor.
     */
    virtual ~ArgumentParser()
    {
    }
    
    /**
     * @copydoc eoos::api::Object::isConstructed()
     */
    virtual bool_t isConstructed() const
    {
        return Parent::isConstructed();
    }
    
    /**
     * @brief Returns list of arguments.
     * 
     * @return List of arguments.
     * 
     * @todo Declare constant function to satisfy MISRA-C++:2008 Rule 9–3–1
     */
    api::List< api::String<T>* >& getArguments()
    {
        return ptrs_;
    }

protected:

    using Parent::setConstructed;

private:

    /**
     * @brief Constructor.
     *
     * @param argc The number of arguments passed to the program.
     * @param argv An array of c-string of arguments where the last one - argc + 1 is null.
     * @return True if object has been constructed successfully.
     */
    bool_t construct(int32_t argc, T* argv[])
    {
        bool_t res( false );
        if( ( !isConstructed() )
         || ( !args_.isConstructed() )
         || ( !addArgs(argc, argv) ) ) 
        {
            res = false;
        }
        else
        {
            res = true;
        }
        return res;
    }

    /**
     * @brief Adds arguments to the list.
     *
     * @param argc The number of arguments passed to the program.
     * @param argv An array of c-string of arguments where the last one - argc + 1 is null.
     * @return True if arguments are added successfully.
     */
    bool_t addArgs(int32_t argc, T* argv[])
    {
        bool_t res( true );
        if( (argc >= 0) && (argv != NULLPTR) )
        {
            for(int32_t i(0); i<argc; i++)
            {
                if( argv[i] != NULLPTR )
                {
                    if( args_.add(argv[i]) == true )
                    {
                        if( ptrs_.add( &args_.getLast() ) == true )
                        {
                            continue;
                        }
                    }
                }
                res = false;
                break;
            }
            if( res == true )
            {
                if( argv[argc] != NULLPTR )
                {
                    res = false;
                }
            }
        }
        else
        {
            res = false;
        }
        return res;
    }   

    /**
     * List to contain program arguments 
     */
    LinkedList<BaseString<T,L,CharTrait<T>,A>,A> args_;

    /**
     * List to contain pointers to program arguments 
     */
    LinkedList<api::String<T>*> ptrs_;

};

} // namespace lib
} // namespace eoos
#endif // LIB_ARGUMENTPARSER_HPP_
