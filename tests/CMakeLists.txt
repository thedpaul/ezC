# Source files for tests
set(TEST_SOURCES
    test_vector.c
    Unity/src/unity.c
)

# Create the test executable
add_executable(tests ${TEST_SOURCES})

include_directories(Unity/src)

# Link against the library
target_link_libraries(tests ezC)

