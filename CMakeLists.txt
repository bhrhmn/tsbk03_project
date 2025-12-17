cmake_minimum_required(VERSION 3.20)
project(SceneProject)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
enable_language(C)

# Find packages
find_package(GLEW REQUIRED)
find_package(SFML 3.0 COMPONENTS Audio Graphics Window System REQUIRED)
find_package(OpenGL REQUIRED)

message(STATUS "GLEW found: ${GLEW_FOUND}")
message(STATUS "SFML found: ${SFML_FOUND}")

# Manually set Assimp paths
set(ASSIMP_INCLUDE_DIR "C:/Users/Luna/Code/assimp-mingw/include")
set(ASSIMP_LIBRARY "C:/Users/Luna/Code/assimp-mingw/lib/libassimp.a")
set(ZLIB_LIBRARY "C:/Users/Luna/Code/assimp-mingw/lib/libzlibstatic.a")  # Add zlib

# Source files
set(SOURCES
        Project/scene.cpp
        Project/camera.cpp
        Project/models.cpp
        Project/rendering.cpp
        Project/lighting.cpp
        Project/shadows.cpp
        Project/tree.cpp
        common/GL_utilities.c
        common/LoadTGA.c
        common/lodepng.cpp
        common/Windows/MicroGlut.c
        Project/doorAnimation.cpp
        Project/doorAnimation.h
        Project/BallAnimation.cpp
        Project/BallAnimation.h
)

# Create executable
add_executable(scene ${SOURCES})

# Include directories
target_include_directories(scene PRIVATE
        common
        common/Windows
        Project
        ${ASSIMP_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(scene PRIVATE
        opengl32
        glu32
        gdi32
        winmm
        GLEW::GLEW
        SFML::Audio
        SFML::Graphics
        SFML::Window
        SFML::System
        ${ASSIMP_LIBRARY}
        ${ZLIB_LIBRARY}  # Link zlib
)

# Windows-specific definitions
if(WIN32)
    target_compile_definitions(scene PRIVATE
            _CRT_SECURE_NO_WARNINGS
            GL_GLEXT_PROTOTYPES
            GLEW_STATIC
    )
endif()

# Debug messages
message(STATUS "Assimp include dir: ${ASSIMP_INCLUDE_DIR}")
message(STATUS "Assimp library: ${ASSIMP_LIBRARY}")
message(STATUS "Zlib library: ${ZLIB_LIBRARY}")
