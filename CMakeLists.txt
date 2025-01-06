cmake_minimum_required(VERSION 3.18)
project(learnopengl)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME})
add_subdirectory(src)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
find_package(OpenGL REQUIRED)
add_subdirectory(ext/glfw)

# Include directories for the project
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/ext/glfw/include
    ${CMAKE_SOURCE_DIR}/ext/glad/include
)
set(glad_SOURCE_DIR glad)

add_library(glad)

target_sources(glad
    PRIVATE
        ${glad_SOURCE_DIR}/src/glad.c
    PUBLIC
        ${glad_SOURCE_DIR}/include/glad/glad.h
        ${glad_SOURCE_DIR}/include/KHR/khrplatform.h
)

target_include_directories(glad PUBLIC ${glad_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL glad)
