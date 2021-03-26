cmake_minimum_required(VERSION 3.10)

add_executable(test_jsmn_parser test_jsmn_parser.c)

target_include_directories(test_jsmn_parser PUBLIC
                          "${PROJECT_SOURCE_DIR}"
                          "${PROJECT_SOURCE_DIR}/jsmn"
                          )
