cmake_minimum_required(VERSION 3.10)

# dirty fix since i dont want to download cmake 3.12 now
add_library(cunit
            "${PROJECT_SOURCE_DIR}/lib/Unity/src/unity.c")

target_include_directories(cunit PUBLIC
                          "${PROJECT_SOURCE_DIR}/lib/Unity/src/"
                          )

add_executable(test_jsmn_parser test_jsmn_parser.c)

target_link_libraries(test_jsmn_parser PUBLIC
                      cunit
                      )

target_include_directories(test_jsmn_parser PUBLIC
                          "${PROJECT_SOURCE_DIR}"
                          "${PROJECT_SOURCE_DIR}/lib/jsmn"
                          )
