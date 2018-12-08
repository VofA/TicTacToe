cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_GENERATOR "ninja")
# set(CMAKE_GENERATOR "make")

# set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_COMPILER "clang-cl")
# set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_COMPILER "clang-cl")

project(App)

file(GLOB_RECURSE SOURCES "*.cpp")

add_definitions(-std=c++17 -g -Wall -Wextra)
# add_definitions(-std=c++17 -g0 -w)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../includes /usr/local/include)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../libraries /usr/local/lib)

add_executable(${PROJECT_NAME} ${SOURCES})

# target_link_libraries(${PROJECT_NAME} pthread ncurses)
target_link_libraries(${PROJECT_NAME})