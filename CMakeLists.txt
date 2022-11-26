# Set the minium cmake version required.
cmake_minimum_required(VERSION 3.10)

# Set the target name
set(TARGET_NAME TerrainGeneration)

message("Building target: ${TARGET_NAME}")

# Set the project name.
project(TerrainGeneration VERSION 1.0)

# Set the standard c++ version to be used.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Include dependencies.
include_directories(${CMAKE_SOURCE_DIR}/dependencies/glfw-3.3.8/include  
                    ${CMAKE_SOURCE_DIR}/dependencies/glad-opengl-4.3/include
                    ${CMAKE_SOURCE_DIR}/dependencies/glm
                    ${CMAKE_SOURCE_DIR}/dependencies/OBJ-Loader/Source
                    ${CMAKE_SOURCE_DIR}/dependencies/stb)

add_subdirectory(src)

