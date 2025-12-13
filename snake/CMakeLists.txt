cmake_minimum_required(VERSION 3.5)
project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SRC_MAIN
        ${PROJECT_SOURCE_DIR}/src/Main.cpp
        include/InputHadler.h
        include/Snake.h
)

add_executable(game ${SRC_MAIN})