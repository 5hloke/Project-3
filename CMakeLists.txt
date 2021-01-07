cmake_minimum_required(VERSION 3.5)
project(P2_1 CXX)

# require a C++11 compiler for all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# build the image library
ADD_SUBDIRECTORY(lib)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

set(STACK_SOURCE
  Stack.hpp Stack.txx)

set(MAZE_SOURCE
  Maze.hpp Maze.cpp)

set(SOURCE ${STACK_SOURCE} ${MAZE_SOURCE})

# create unittests
add_executable(student-tests catch.hpp student_catch.cpp ${SOURCE} student_tests.cpp)
TARGET_LINK_LIBRARIES(student-tests image)

# some simple tests
enable_testing()
add_test(student-tests student-tests)

add_custom_target(submission COMMAND
  ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_CURRENT_BINARY_DIR}/submission.zip" --format=zip
  Maze.hpp Maze.cpp student_tests.cpp
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
