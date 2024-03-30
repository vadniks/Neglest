
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

add_compile_options("-Wno-nullability-extension")

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
file(GLOB GLAD_SOURCES CONFIGURE_DEPENDS glad/include/*/* glad/src/*)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${GLAD_SOURCES})

target_link_libraries(${PROJECT_NAME} SDL2 SDL2_ttf SDL2_image glm)
target_include_directories(${PROJECT_NAME} PRIVATE glad/include)
