cmake_minimum_required(VERSION 3.26)
project(backgammon C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${app_SOURCE_DIR}/cmake")
set(BIN_DIR ${app_SOURCE_DIR}/bin)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

include_directories("./src")
file(GLOB app_SOURCES src/*.c)

add_executable(backgammon ${app_SOURCES}
        src/board.h
        src/board.c
        src/field.h
        src/field.c
        src/game.c
        src/game.h
        src/move.h
)
target_link_libraries(backgammon ${CURSES_LIBRARY})
install(TARGETS backgammon RUNTIME DESTINATION ${BIN_DIR})