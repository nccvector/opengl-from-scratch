
set (IMGUI_PATH ext/imgui)
include_directories(${IMGUI_PATH})
set(IMGUI_SRSC
        ${IMGUI_PATH}/imgui.cpp
        ${IMGUI_PATH}/imgui_draw.cpp
        ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/imgui_impl_glfw.cpp
        ${IMGUI_PATH}/imgui_impl_opengl3.cpp
        ${IMGUI_PATH}/imgui_tables.cpp
        ${IMGUI_PATH}/imgui_widgets.cpp)