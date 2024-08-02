cmake_minimum_required(VERSION 3.12)
project(Fractals)

# Find the required packages
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Set the C++ standard to C++11
set(CMAKE_C_STANDARD 99)

# Add the Malbrot target
set(GUI_SRC src/gui.c)
set(CLI_SRC src/cli.c)

# Add source files for the math target
file(GLOB MATH_SRC src/math/*.c)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files for the CLI target
add_executable(cli ${CLI_SRC} ${MATH_SRC})
target_link_libraries(cli m)

# Define the CLI target
#target_compile_definitions(cli PRIVATE CLI_DERIVATIVE_OPTIMIZATION CLI_TARGET_PNG)

# Add source files for the GUI target
add_executable(gui ${GUI_SRC} ${MATH_SRC})
target_link_libraries(gui m glfw OpenGL::GL GLEW::GLEW)