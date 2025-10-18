cmake_minimum_required(VERSION 3.10)

project(effects)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT(CMAKE_BUILD_TYPE STREQUAL "Debug"))
    add_compile_options(-O3)
endif()

link_directories("/usr/local/lib")

set(3RD_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/3rd)

if(MSVC)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/opencv-windows.cmake)
else()
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/opencv-desktop.cmake)
endif()

file(GLOB SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)

include_directories(${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
include_directories(3rd/opencv/include)
add_executable(effect ${SOURCES})
target_link_libraries(effect opencv)

target_compile_definitions(effect PRIVATE -DMIN=std::min)
target_compile_definitions(effect PRIVATE -DMAX=std::max)


add_custom_target(createAssetsBundle ALL
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_LIST_DIR}/../frames ${CMAKE_CURRENT_BINARY_DIR}/frames)
