cmake_minimum_required(VERSION 3.15)
project(FocusPro)

set(CMAKE_CXX_STANDARD 17)

# SFML Static build (වැදගත්ම දේ)
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

add_executable(FocusPro main.cpp)
target_link_libraries(FocusPro PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)