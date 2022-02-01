cmake_minimum_required(VERSION 3.16)
project(game-app)

set(CMAKE_CXX_STANDARD 17)

set(SFML_INCLUDE_DIR "/home/dbouchou/projets/IN204_projet_IA_game/SFML-2.5.1-Compiled/include")
set(SFML_LIBRARY_DIR "/home/dbouchou/projets/IN204_projet_IA_game/SFML-2.5.1-Compiled/lib")
set(SFML_DIR "/home/dbouchou/projets/IN204_projet_IA_game/SFML-2.5.1-Compiled/lib/cmake/SFML")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
set(Torch_DIR "/home/dbouchou/projets/IN204_projet_IA_game/libtorch/share/cmake/Torch")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)
find_package(Torch REQUIRED)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

if(SFML_FOUND)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
endif()

add_executable(game-app main.cpp Game.cpp Player.cpp Fireball.cpp Enemy.cpp EnemyRock.cpp)
target_link_libraries (game-app sfml-graphics sfml-window sfml-system sfml-network sfml-audio )
