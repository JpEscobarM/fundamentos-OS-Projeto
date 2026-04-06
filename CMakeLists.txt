cmake_minimum_required(VERSION 3.16)

project(analise-iot C)

set(CMAKE_C_STANDARD 99)

find_package(Threads REQUIRED)

add_executable(analise
        src/main.c
        src/logger.c
        src/file.c
        src/cJSON.c)

target_include_directories(analise PRIVATE include)

target_link_libraries(analise Threads::Threads)

target_compile_definitions(analise PRIVATE
        DATA_DIR="${CMAKE_SOURCE_DIR}/input"
)