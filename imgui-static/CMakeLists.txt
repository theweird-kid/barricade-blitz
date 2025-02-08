cmake_minimum_required(VERSION 3.10)
project(ImGui)

# Add source files
set(IMGUI_SRC
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_impl_sdl2.cpp
    imgui/imgui_impl_sdlrenderer2.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
)

# Add header files
set(IMGUI_HEADERS
    include/imgui/imgui.h
    include/imgui/imconfig.h
    include/imgui/imgui_impl_sdl2.h
    include/imgui/imgui_impl_sdlrenderer2.h
    include/imgui/imgui_internal.h
    include/imgui/imstb_rectpack.h
    include/imgui/imstb_textedit.h
    include/imgui/imstb_truetype.h
)

# Find SDL2 package
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Create a shared library
add_library(imgui_shared SHARED ${IMGUI_SRC})

# Specify include directories
target_include_directories(imgui_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui)

# Link SDL2 to the shared library
target_link_libraries(imgui_shared ${SDL2_LIBRARIES})

# Installation rules
install(TARGETS imgui_shared
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/imgui/ DESTINATION include/imgui)
