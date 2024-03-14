cmake_minimum_required(VERSION 3.0.0)

# Specify project and languages (notice: CUDA)
project(02-simple-triangle LANGUAGES C CXX)

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

# Adds a command to the target. Everytime the target "make" is called, the shaders will be copied
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
        COMMENT "Copying shaders" VERBATIM
)