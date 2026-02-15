cmake_minimum_required(VERSION 3.15)
project(FocusPro)

set(CMAKE_CXX_STANDARD 17)

# මෙන්න මෙතනින් තමයි අර MT/MD අවුල විසඳන්නේ
# මේකෙන් අපේ project එකත් SFML වගේම Static Runtime (MT) වලට හැරෙනවා
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# SFML 3.x සොයා ගැනීම
find_package(SFML 3 CONFIG REQUIRED COMPONENTS Graphics Window System)

add_executable(FocusPro WIN32 main.cpp)

# Static linking flags
target_compile_definitions(FocusPro PRIVATE SFML_STATIC)

# Linking libraries
target_link_libraries(FocusPro PRIVATE SFML::Graphics SFML::Window SFML::System)