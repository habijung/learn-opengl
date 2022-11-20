cmake_minimum_required(VERSION 3.15)
project(Learn-OpenGL)
set(CMAKE_CXX_STANDARD 17)

# OpenGL
find_package(OpenGL REQUIRED)

# GLAD
include_directories(external/glad/include)

# GLFW
# GLFW의 docs, tests, examples를 빌드하지 않는 옵션
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)
include_directories(external/glfw)

# GLM

# ASSIMP

# Project execute
add_executable(${PROJECT_NAME}
        external/glad/src/glad.c
        main.cpp
        stb_image.h
        )
target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        glfw
        )
