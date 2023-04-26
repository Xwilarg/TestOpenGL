cmake_minimum_required(VERSION 3.13)
project(TestOpenGL)

SET(CMAKE_C_FLAGS "/W4 /EHsc")
aux_source_directory(. SRCS)
add_executable(TestOpenGL WIN32 ${SRCS})
target_link_libraries(TestOpenGL OpenGL32)