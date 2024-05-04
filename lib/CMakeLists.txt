
add_compile_options("-fvisibility=hidden")
file(GLOB LIB_SOURCES CONFIGURE_DEPENDS src/*.c src/*.h)
add_library(${LIB} SHARED ${LIB_SOURCES} include)
