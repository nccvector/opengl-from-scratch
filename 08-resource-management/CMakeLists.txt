cmake_minimum_required(VERSION 3.0.0)

# Specify project and languages (notice: CUDA)
project(08-resource-management LANGUAGES C CXX)

# Find OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# Add glfw3
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw3)

# Add glad
add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)

# Add fbxsdk
add_subdirectory(fbxsdk)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
        ${OPENGL_INCLUDE_DIR}
        ${FBXSDK_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/glm
)

# Add cpp and cu files to target. CMake will automatically compile cu with nvcc
add_executable(${PROJECT_NAME}
        main.cpp
        Shader.cpp
        utils.cpp
        ModelTools.cpp
        Camera.cpp
        common.cpp
        ResourceManager.cpp
)

target_link_libraries(${PROJECT_NAME}
       ${OPENGL_LIBRARIES}
        glad
        glfw
        ${FBXSDK_LIBRARY}
)

configure_file(shaders/shader.frag ${CMAKE_BINARY_DIR}/shaders/shader.frag)
configure_file(shaders/shader.vert ${CMAKE_BINARY_DIR}/shaders/shader.vert)

add_custom_target(copy-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_BINARY_DIR}/resources
)
add_dependencies(${PROJECT_NAME} copy-files)
