# cmake_minimum_required(VERSION 3.18)
# project(learnopengl)

# # Generate compile_commands.json
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add_executable(${PROJECT_NAME})
# add_subdirectory(src)

# set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
# set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
# find_package(OpenGL REQUIRED)
# add_subdirectory(ext/glfw)

# # Include directories for the project
# target_include_directories(${PROJECT_NAME} PRIVATE
#     ${CMAKE_SOURCE_DIR}/ext/glfw/include
#     ${CMAKE_SOURCE_DIR}/ext/glad/include
# )
# set(glad_SOURCE_DIR glad)

# add_library(glad)

# target_sources(glad
#     PRIVATE
#         ${glad_SOURCE_DIR}/src/glad.c
#     PUBLIC
#         ${glad_SOURCE_DIR}/include/glad/glad.h
#         ${glad_SOURCE_DIR}/include/KHR/khrplatform.h
# )

# target_include_directories(glad PUBLIC ${glad_SOURCE_DIR}/include)

# # Link libraries
# target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL glad)

cmake_minimum_required(VERSION 3.18)
project(learnopengl)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define a function to add an executable with common properties
function(add_custom_executable target_name source_dir)
    add_executable(${target_name} ${source_dir}/main.cpp)

    add_subdirectory(${source_dir})
    set_target_properties(${target_name} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
    set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${target_name})

    target_include_directories(${target_name} PRIVATE
        ${CMAKE_SOURCE_DIR}/ext/glfw/include
        ${CMAKE_SOURCE_DIR}/ext/glad/include
    )

    target_link_libraries(${target_name} glfw OpenGL::GL glad)
endfunction()

# Add the main executable
add_custom_executable(ex1 src/ex1)


# Add another executable
# add_custom_executable(another_executable src/ex1/main.cpp)
add_custom_executable(ex2 src/ex2)

# Add subdirectories
add_subdirectory(ext/glfw)

# Find OpenGL package
find_package(OpenGL REQUIRED)

# Set up glad library
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
