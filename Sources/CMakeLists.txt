<<<<<<< HEAD
cmake_minimum_required(VERSION 3.18)
=======
cmake_minimum_required(VERSION 3.16)
>>>>>>> 953090755d10ddadf380cb71be8d4b162cdb88f1
project(game-app)

set(CMAKE_CXX_STANDARD 17)

<<<<<<< HEAD
set(SFML_INCLUDE_DIR "${CMAKE_BINARY_DIR}/../SFML-2.5.1-Compiled/include")
set(SFML_LIBRARY_DIR "${CMAKE_BINARY_DIR}/../SFML-2.5.1-Compiled/lib")
set(SFML_DIR "${CMAKE_BINARY_DIR}/../SFML-2.5.1-Compiled/lib/cmake/SFML")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)
=======
set(MY_DIR "")

set(SFML_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../SFML-2.5.1-Compiled/include")
set(SFML_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/../SFML-2.5.1-Compiled/lib")
#set(TORCH_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../libtorch/include")
#set(TORCH_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/../libtorch/lib")
set(SFML_DIR "${CMAKE_SOURCE_DIR}/../SFML-2.5.1-Compiled/lib/cmake/SFML")
set(Torch_DIR "${CMAKE_SOURCE_DIR}/../libtorch/share/cmake/Torch")

link_directories(SFML_LIBRARY_DIR TORCH_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR TORCH_INCLUDE_DIR)


find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)
find_package(Torch)


add_executable(game-app Enemy.cpp EnemyRock.cpp Fireball.cpp Game.cpp main.cpp Player.cpp)

target_link_libraries(game-app sfml-graphics sfml-window sfml-system sfml-network sfml-audio)
set_property(TARGET game-app PROPERTY CXX_STANDARD 17)
>>>>>>> 953090755d10ddadf380cb71be8d4b162cdb88f1

if(SFML_FOUND)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
endif()

<<<<<<< HEAD
add_executable(game-app Enemy.cpp EnemyRock.cpp Player.cpp Game.cpp Fireball.cpp main.cpp)
target_link_libraries (game-app sfml-graphics sfml-window sfml-system)
=======
# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

>>>>>>> 953090755d10ddadf380cb71be8d4b162cdb88f1
