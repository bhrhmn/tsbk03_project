cmake_minimum_required(VERSION 3.15)
project(scene)

set(CMAKE_CXX_STANDARD 17)
set(commondir ../common)

# Add executable
add_executable(scene
        scene.cpp
        ${commondir}/GL_utilities.c
        ${commondir}/LoadTGA.c
        ${commondir}/Windows/MicroGlut.c
        ${commondir}/lodepng.cpp
        scene.cpp
        camera.cpp
        tree.cpp
        models.cpp
        rendering.h
        lighting.cpp
        rendering.cpp
        shadows.cpp
        sounds.cpp
        sounds.h
)

# Include directories
target_include_directories(scene PRIVATE ${commondir} ${commondir}/Windows)

# Try to find SFML, fallback to basic OpenGL
find_package(SFML COMPONENTS audio graphics window system QUIET)

if(SFML_FOUND)
    target_link_libraries(scene SFML::Audio SFML::Graphics SFML::Window SFML::System)
    message(STATUS "SFML found - audio enabled")
else()
    target_link_libraries(scene opengl32 glu32)
    message(WARNING "SFML not found - building without audio")
endif()

target_link_libraries(scene opengl32 glu32)
target_compile_definitions(scene PRIVATE GL_GLEXT_PROTOTYPES)