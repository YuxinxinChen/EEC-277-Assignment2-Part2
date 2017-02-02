cmake_minimum_required(VERSION 3.6)
project(OpenglTest CXX C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)


find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)

include_directories(${GLFW_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(/Users/Nanaly/GLFW/glfw-3.2.1/deps)
add_executable(OpenglTest ${SOURCE_FILES})

target_link_libraries(OpenglTest ${GLEW_LIBRARIES} ${GLFW_LIBRARY} ${OPENGL_LIBRARY})
