cmake_minimum_required(VERSION 3.0.0)
project(opengl-from-scratch)

set(OpenGL_GL_PREFERENCE "GLVND")

# optix stuff
include(optix/CMakeLists.txt)
cuda_compile_and_embed(embedded_ptx_code optix/DevicePrograms.cu)

# Add opengl and glfw
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Adding glm
add_subdirectory(ext/glm)

# Adding assimp using the find_package
find_package(assimp PATHS ext/assimp REQUIRED)

# Setup imgui
include_directories(ext/imgui)
set(IMGUI_SRCS
        ext/imgui/imgui.cpp
        ext/imgui/imgui_widgets.cpp
        ext/imgui/imgui_tables.cpp
        ext/imgui/imgui_impl_opengl3.cpp
        ext/imgui/imgui_impl_glfw.cpp
        ext/imgui/imgui_demo.cpp
        ext/imgui/imgui_draw.cpp)

set(INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/ext/glm/
        ${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/include
        ${ASSIMP_INCLUDE_DIRS})

message("INCLUDE_DIRS: " ${INCLUDE_DIRS})

# include opengl wrangler
include_directories(${INCLUDE_DIRS})

# Target ofs
add_executable(ofs
        ${embedded_ptx_code}
        ${IMGUI_SRCS}
        main.cpp
        ext/glad/src/glad.c
        Shader.cpp
        Shader.h
        Texture.cpp
        Texture.h
        Model.cpp
        Model.h
        Types.h
        Types.cpp
        Vertex.cpp
        Vertex.h
        Material.cpp
        Material.h
        Light.h
        PhongShader.h
        PhongShader.cpp
        optix/Scene.cpp
        optix/Scene.h
        optix/LaunchParams.h)
target_link_libraries(ofs
        ${OptiX_LIBS}
        ${OPENGL_LIBRARIES}
        glfw
        ${ASSIMP_LIBRARIES})

# Copy shaders to bin folder
file(COPY shaders DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY textures DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY models DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
