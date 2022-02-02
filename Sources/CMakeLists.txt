cmake_minimum_required(VERSION 3.18)
project(gameapp)

set(CMAKE_CXX_STANDARD 17)

set(SFML_INCLUDE_DIR "${CMAKE_BINARY_DIR}/../SFML-2.5.1-Compiled/include")
set(SFML_LIBRARY_DIR "${CMAKE_BINARY_DIR}/../SFML-2.5.1-Compiled/lib")
set(SFML_DIR "${CMAKE_BINARY_DIR}/../SFML-2.5.1-Compiled/lib/cmake/SFML")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

if(SFML_FOUND)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
endif()

add_executable(game-app main.cpp Game.cpp Player.cpp Fireball.cpp Enemy.cpp EnemyRock.cpp)
target_link_libraries (game-app sfml-graphics sfml-window sfml-system)