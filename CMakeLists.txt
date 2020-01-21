# EOOS RT LIBRARY.
#
# @author    Sergey Baigudin, sergey@baigudin.software
# @copyright 2019, Sergey Baigudin, Baigudin Software
# @license   http://embedded.team/license/

target_include_directories(target-eoos
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)
