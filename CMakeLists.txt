cmake_minimum_required(VERSION 3.16)

project(tcb C)

set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

add_executable(tcb ${SRC_FILES})

target_include_directories(tcb PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)