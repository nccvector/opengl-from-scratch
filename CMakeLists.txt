cmake_minimum_required(VERSION 3.0.0)
project(opengl-from-scratch)

set(OpenGL_GL_PREFERENCE "GLVND")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# include opengl wrangler
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/include)

add_executable(ofs main.cpp ext/glad/src/glad.c)
target_link_libraries(ofs ${OPENGL_LIBRARIES} glfw)

file(COPY shaders DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
