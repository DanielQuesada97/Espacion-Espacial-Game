cmake_minimum_required(VERSION 3.31)
project(estacion_espacial_v3)

set(CMAKE_CXX_STANDARD 20)

# Use SFML 3.0.1 with required components
set(SFML_DIR "/usr/local/Cellar/sfml/3.0.1/lib/cmake/SFML")
find_package(SFML 3.0 REQUIRED COMPONENTS Window Graphics System)

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/MapManager.cpp
    src/AIBot.cpp
    src/Renderer.cpp
    src/InputHandler.cpp
)

add_executable(estacion_espacial_v3 ${SOURCES})

# Include directories
target_include_directories(estacion_espacial_v3 PRIVATE include)

# Use modern CMake target syntax for linking SFML
target_link_libraries(estacion_espacial_v3 PRIVATE
        SFML::Window
        SFML::Graphics
        SFML::System
)

# Set compiler flags for macOS
if(APPLE)
    target_compile_options(estacion_espacial_v3 PRIVATE -std=c++20)
endif()

# Copy maps directory to build directory
add_custom_command(TARGET estacion_espacial_v3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/maps
    ${CMAKE_BINARY_DIR}/maps
)