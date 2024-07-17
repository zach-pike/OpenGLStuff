cmake_minimum_required(VERSION  3.30.0)

set(IMGUI_SOURCE 
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui.cpp
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
)
add_library(IMGui STATIC ${IMGUI_SOURCE})
target_include_directories(IMGui AFTER PUBLIC .)
target_include_directories(IMGui AFTER PUBLIC ../)