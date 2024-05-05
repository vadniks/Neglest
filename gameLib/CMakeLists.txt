
file(GLOB GAME_LIB_SOURCES CONFIGURE_DEPENDS src/*.c src/*.h)
add_library(${GAME_LIB} SHARED ${GAME_LIB_SOURCES} include)

target_compile_options(${GAME_LIB} PRIVATE "-fvisibility=hidden")

target_link_libraries(${GAME_LIB} SDL2)
target_include_directories(${GAME_LIB} PRIVATE include)
