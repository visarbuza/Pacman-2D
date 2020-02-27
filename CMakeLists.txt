cmake_minimum_required (VERSION 3.14 FATAL_ERROR)
project(Pacman VERSION 0.1)

# Add dependencies
add_subdirectory(external)

# Create .exe file (essentially)
add_executable(${PROJECT_NAME})

# Add additional source files here
target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    
    ${CMAKE_CURRENT_LIST_DIR}/include/game.h
    ${CMAKE_CURRENT_LIST_DIR}/src/framework/core/game.cpp
    
    ${CMAKE_CURRENT_LIST_DIR}/include/resource_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/src/framework/core/resource_manager.cpp

    ${CMAKE_CURRENT_LIST_DIR}/include/shader.h
    ${CMAKE_CURRENT_LIST_DIR}/src/framework/rendering/shader.cpp

    ${CMAKE_CURRENT_LIST_DIR}/include/texture.h
    ${CMAKE_CURRENT_LIST_DIR}/src/framework/rendering/texture.cpp
    
    ${CMAKE_CURRENT_LIST_DIR}/include/sprite_renderer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/framework/rendering/sprite_renderer.cpp

    ${CMAKE_CURRENT_LIST_DIR}/include/game_object.h
    ${CMAKE_CURRENT_LIST_DIR}/src/framework/core/game_object.cpp

    ${CMAKE_CURRENT_LIST_DIR}/include/game_level.h
    ${CMAKE_CURRENT_LIST_DIR}/src/framework/core/game_level.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imconfig.h
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui.h
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_impl_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_impl_opengl3.h
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_internal.h
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_widgets.cpp

)

# Set include directories
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/src
)

# Link with external libraries
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    glad::glad
    gfx::gfx
    glm
    glfw
    stbimage::stbimage
)

# Enable C++ 17
target_compile_features(
    ${PROJECT_NAME}
    PRIVATE
    cxx_std_17
)

# Copy resources to binary directory
add_custom_target(
    copy_shaders ALL
    ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMAND ${CMAKE_COMMAND} -E echo "Copied resource directory to binary directory ${CMAKE_CURRENT_BINARY_DIR}"
)

