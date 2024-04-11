
cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
project(Neglest LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_compile_options("-Wall")
add_compile_options("-Wextra")
add_compile_options("-Wpedantic")
add_compile_options("-O3")
#add_link_options("-s")

add_compile_options("-Wno-c99-extensions")

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

set(CGLM_PATH ${PROJECT_SOURCE_DIR}/libs/cglm)

target_link_libraries(${PROJECT_NAME} SDL2 SDL2_ttf SDL2_image GL GLEW glm ${CGLM_PATH}/bin/libcglm.so)
include_directories(${CGLM_PATH}/include)

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY images DESTINATION ${CMAKE_BINARY_DIR})
file(COPY font DESTINATION ${CMAKE_BINARY_DIR})
