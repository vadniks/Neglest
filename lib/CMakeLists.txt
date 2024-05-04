
file(GLOB LIB_SOURCES CONFIGURE_DEPENDS src/*.c src/*.h)
add_library(${LIB} SHARED ${LIB_SOURCES} include)
target_compile_options(${LIB} PRIVATE "-fvisibility=hidden")
