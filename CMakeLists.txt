cmake_minimum_required(VERSION 3.0.0)

# Specify project and languages (notice: CUDA)
project(dev LANGUAGES C CXX)

# Find OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# Add glfw3
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw3)

# Add glad
add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c)

# Add fbxsdk
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/fbxsdk)

# Add spdlog
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog)

# Add imgui
set(IMGUI_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/cpp
)

set(IMGUI_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/misc/cpp/imgui_stdlib.cpp
)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/external
        ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
        ${OPENGL_INCLUDE_DIR}
        ${FBXSDK_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog/include
        ${IMGUI_INCLUDE_DIRS}
)

# Add cpp and cu files to target. CMake will automatically compile cu with nvcc
add_executable(${PROJECT_NAME}
        main.cpp
        Shader.cpp
        Camera.cpp
        Common.cpp
        ResourceManager.cpp
        Texture.cpp
        Material.cpp
        Mesh.cpp
        Model.cpp
        WindowManager.cpp
        Application.cpp
        ${IMGUI_SRCS}
)

target_link_libraries(${PROJECT_NAME}
       ${OPENGL_LIBRARIES}
        glad
        glfw
        ${FBXSDK_LIBRARY}
        spdlog
)

configure_file(shaders/shader.frag ${CMAKE_BINARY_DIR}/shaders/shader.frag)
configure_file(shaders/shader.vert ${CMAKE_BINARY_DIR}/shaders/shader.vert)

add_custom_target(copy-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_BINARY_DIR}/resources
)
add_dependencies(${PROJECT_NAME} copy-files)
