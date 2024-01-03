# Source files
set(SOURCES
    arena.c
    iterator.c
    list_iterator.c
    vector.c
)

# Create a library from the source files
add_library(ezC STATIC ${SOURCES})

# Specify include directories
target_include_directories(ezC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

