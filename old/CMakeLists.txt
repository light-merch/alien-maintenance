cmake_minimum_required(VERSION 3.17)
project(alien_maintenance)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(alien_maintenance main.cpp)

target_link_libraries(alien_maintenance sfml-graphics sfml-audio)