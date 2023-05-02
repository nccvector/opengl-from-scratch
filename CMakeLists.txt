cmake_minimum_required(VERSION 3.0.0)
project(opengl-from-scratch)

set(OpenGL_GL_PREFERENCE "GLVND")

# Add opengl and glfw
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Adding glm
add_subdirectory(ext/glm)

# Adding assimp using the find_package
find_package(assimp PATHS ext/assimp REQUIRED)

set(INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/ext/glm/
        ${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/include
        ${ASSIMP_INCLUDE_DIRS})

message("INCLUDE_DIRS: " ${INCLUDE_DIRS})

# include opengl wrangler
include_directories(${INCLUDE_DIRS})

# Target ofs
add_executable(ofs main.cpp ext/glad/src/glad.c Shader.cpp Texture.cpp Model.cpp Model.h Types.h Types.cpp Vertex.cpp Vertex.h)
target_link_libraries(ofs
        ${OPENGL_LIBRARIES}
        glfw
        ${ASSIMP_LIBRARIES})

# Copy shaders to bin folder
file(COPY shaders DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY textures DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY models DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
