# EOOS RT LIBRARY.
#
# @author    Sergey Baigudin, sergey@baigudin.software
# @copyright 2019, Sergey Baigudin, Embedded Team
# @license   http://embedded.team/license/

enable_language(CXX)

target_include_directories(eoos
INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)
