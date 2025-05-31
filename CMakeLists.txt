#
#   project
#

cmake_minimum_required(VERSION 3.20)
project(rias C)

set(common_bin_dir ${CMAKE_SOURCE_DIR}/bin/$<0:>)

#
#   tests
#

file(GLOB_RECURSE test_files CONFIGURE_DEPENDS tests/*.c)
add_executable(tests ${test_files})

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

set_target_properties(tests PROPERTIES
    C_STANDARD 17
    RUNTIME_OUTPUT_DIRECTORY ${common_bin_dir}
)
