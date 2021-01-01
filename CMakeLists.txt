# EOOS RT LIBRARY.
#
# @author    Sergey Baigudin, sergey@baigudin.software
# @copyright 2019-2020, Sergey Baigudin, Baigudin Software

target_include_directories(target-eoos
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/public>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/public/auxiliary>    
)
