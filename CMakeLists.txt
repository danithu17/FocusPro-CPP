cmake_minimum_required(VERSION 3.15)
project(FocusPro)

set(CMAKE_CXX_STANDARD 17)

# SFML 3.x Compatibility
find_package(SFML 3 CONFIG REQUIRED COMPONENTS Graphics Window System)

add_executable(FocusPro main.cpp)

# Static link logic for vcpkg
target_link_libraries(FocusPro PRIVATE SFML::Graphics SFML::Window SFML::System)