cmake_minimum_required(VERSION 3.20)
project(SceneProject)

# Set vcpkg toolchain
#set(CMAKE_TOOLCHAIN_FILE "C:/Users/lunas/Documents/tsbk03_project/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_TOOLCHAIN_FILE "C:/Users/jorun/Kod/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_CXX_STANDARD 17)
enable_language(C)

# Find packages - ADD GLEW
find_package(GLEW REQUIRED)
find_package(SFML 3.0 COMPONENTS Audio Graphics Window System REQUIRED)
find_package(OpenGL REQUIRED)

message(STATUS "GLEW found: ${GLEW_FOUND}")
message(STATUS "SFML found: ${SFML_FOUND}")

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
)

# Create executable
add_executable(scene ${SOURCES})

# Link libraries - ADD GLEW


# Windows-specific libraries
if(WIN32)
    target_link_libraries(scene
            opengl32
            glu32
            gdi32
            winmm
            GLEW::GLEW
    )

    target_compile_definitions(scene PRIVATE
            _CRT_SECURE_NO_WARNINGS
            GL_GLEXT_PROTOTYPES
            GLEW_STATIC      # Add this for static linking
    )
endif()

# Include directories
target_include_directories(scene PRIVATE
        common
        common/Windows
        Project
)