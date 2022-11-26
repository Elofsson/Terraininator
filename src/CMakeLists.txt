# Set the minium cmake version required.
cmake_minimum_required(VERSION 3.10)

# Find all source files
file(GLOB_RECURSE SOURCE *.cpp *.h)

# Create an executable from the sources
add_executable(${TARGET_NAME} ${SOURCE} ${CMAKE_SOURCE_DIR}/dependencies/glad-opengl-4.3/src/glad.c)

# TODO Maybe add cmake find modules to get all the locations for the libraries to link.
# This executable requires a few libraries to link 
target_link_libraries(${TARGET_NAME} ${CMAKE_SOURCE_DIR}/dependencies/glfw-3.3.8/build/src/libglfw3.a -lopengl32)

# TODO maybe call install here.