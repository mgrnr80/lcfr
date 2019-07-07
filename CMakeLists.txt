cmake_minimum_required (VERSION 2.6)
project (lcfr)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRCS src/*.cpp)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_definitions(-D__BUILD_LCFR_LIBRARY__)

find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

add_library(lcfr SHARED ${SRCS})
target_link_libraries(lcfr -static-libgcc -static-libstdc++)
