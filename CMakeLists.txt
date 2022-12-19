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
add_subdirectory(external/glm)
include_directories(external/glm)

# ASSIMP

# Custom Include
include_directories(include)

# Project execute
add_executable(${PROJECT_NAME}
        external/glad/src/glad.c
        src/1.7.Camera.cpp
        )
target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        glfw
        )
