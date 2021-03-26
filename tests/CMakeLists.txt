cmake_minimum_required(VERSION 3.10)

# dirty fix since i dont want to download cmake 3.12 now
add_library(cunit
	"${PROJECT_SOURCE_DIR}/lib/Unity/src/unity.c"
	)

target_include_directories(cunit PUBLIC
	"${PROJECT_SOURCE_DIR}/lib/Unity/src/"
	)

add_executable(test_jsmn_parser test_jsmn_parser.c)
add_definitions(-fprofile-arcs -ftest-coverage)

target_link_libraries(test_jsmn_parser PUBLIC
	cunit
	-fprofile-arcs
	)

target_include_directories(test_jsmn_parser PUBLIC
	"${PROJECT_SOURCE_DIR}"
	"${PROJECT_SOURCE_DIR}/lib/jsmn"
	)

set(OBJECT_DIR ${CMAKE_BINARY_DIR}/tests/CMakeFiles/test_jsmn_parser.dir)

add_custom_target(lcov
    COMMAND mkdir -p coverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)

add_custom_command(TARGET lcov
    COMMAND lcov -c -d ${OBJECT_DIR} -o test.info
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/coverage  # Need separate command for this line
    )
