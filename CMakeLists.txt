cmake_minimum_required(VERSION 3.17)
project(opengl_example)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)

add_executable(opengl_example main.cpp)

target_link_libraries(opengl_example ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})