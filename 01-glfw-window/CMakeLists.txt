cmake_minimum_required(VERSION 3.0.0)

# Specify project and languages (notice: CUDA)
project(01-glfw-window LANGUAGES C CXX)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add glfw3
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw3)

# Add glad
add_library(glad
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c
)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
        ${OPENGL_INCLUDE_DIR}
)

# Add cpp and cu files to target. CMake will automatically compile cu with nvcc
add_executable(${PROJECT_NAME}
        main.cpp
)

target_link_libraries(${PROJECT_NAME}
       ${OPENGL_LIBRARIES}
        glad
        glfw
)
