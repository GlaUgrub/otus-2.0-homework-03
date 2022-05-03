# setup project
cmake_minimum_required(VERSION 3.14)

project(otus_2_0_homework_03 VERSION 0.0.1)

# build ip_filter

include(conanbuildinfo.cmake)
conan_basic_setup()

add_executable(ip_filter_range_app ip_filter_range.cpp ip_tools.cpp)

set_target_properties(ip_filter_range_app PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

