cmake_minimum_required(VERSION 3.16)
project(tcb C)

set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SRC_TCB_DAEMON_FILES "${PROJECT_SOURCE_DIR}/src/tcb-daemon/*.c")
file(GLOB SRC_TCB_PUSH_FILES   "${PROJECT_SOURCE_DIR}/src/tcb-push/*.c")
file(GLOB SRC_TCB_PULL_FILES   "${PROJECT_SOURCE_DIR}/src/tcb-pull/*.c")

add_executable(tcb-daemon ${SRC_TCB_DAEMON_FILES})
target_include_directories(tcb-daemon PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_executable(tcb-push ${SRC_TCB_PUSH_FILES})
target_include_directories(tcb-push PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_executable(tcb-pull ${SRC_TCB_PULL_FILES})
target_include_directories(tcb-pull PRIVATE ${PROJECT_SOURCE_DIR}/include) 