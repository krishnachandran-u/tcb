cmake_minimum_required(VERSION 3.16)
project(tcb C)

set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(X11 REQUIRED IMPORTED_TARGET x11 xfixes)

file(GLOB SRC_TCB_DAEMON_FILES "${PROJECT_SOURCE_DIR}/src/tcb-daemon/*.c")
file(GLOB SRC_TCB_PUSH_FILES   "${PROJECT_SOURCE_DIR}/src/tcb-push/*.c")
file(GLOB SRC_TCB_PULL_FILES   "${PROJECT_SOURCE_DIR}/src/tcb-pull/*.c")

add_executable(tcb-daemon ${SRC_TCB_DAEMON_FILES})
target_include_directories(tcb-daemon PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_executable(tcb-push ${SRC_TCB_PUSH_FILES})
target_include_directories(tcb-push PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_executable(tcb-pull ${SRC_TCB_PULL_FILES})
target_include_directories(tcb-pull PRIVATE ${PROJECT_SOURCE_DIR}/include) 

add_executable(tcb-clipnotify "${PROJECT_SOURCE_DIR}/src/clipnotify/clipnotify.c")
target_include_directories(tcb-clipnotify PRIVATE ${X11_INCLUDE_DIR})
target_link_libraries(tcb-clipnotify PRIVATE PkgConfig::X11)

include(GNUInstallDirs)

install(TARGETS tcb-daemon tcb-push tcb-pull tcb-clipnotify
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(PROGRAMS tcb.sh 
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        RENAME tcb)

install(PROGRAMS tcb-watch.sh 
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        RENAME tcb-watch)