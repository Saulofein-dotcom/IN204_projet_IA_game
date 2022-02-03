cmake_minimum_required(VERSION 3.16)
project(gameapp)

set(CMAKE_CXX_STANDARD 17)

set(SFML_INCLUDE_DIR "${CMAKE_BINARY_DIR}/../SFML-2.5.1-Compiled/include")
set(SFML_LIBRARY_DIR "${CMAKE_BINARY_DIR}/../SFML-2.5.1-Compiled/lib")
set(SFML_DIR "${CMAKE_BINARY_DIR}/../SFML-2.5.1-Compiled/lib/cmake/SFML")
set(Torch_DIR "/home/bdjessim/projets/IN204_projet_IA_game/libtorch/share/cmake/Torch")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)
find_package(Torch REQUIRED)

if(SFML_FOUND)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
endif()

add_executable(game-app test.cpp)
target_link_libraries (game-app "${TORCH_LIBRARIES}" sfml-graphics)