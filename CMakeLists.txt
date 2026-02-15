cmake_minimum_required(VERSION 3.15)
project(FocusPro-CPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static Runtime එක අනිවාර්යයි
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# SFML 3.0 සෙවීම
find_package(SFML 3.0 REQUIRED COMPONENTS graphics window system)

# GUI App එකක් ලෙස නිර්මාණය (CMD Window එක අයින් වේ)
add_executable(FocusPro WIN32 main.cpp)

# Libraries ලින්ක් කිරීම
target_link_libraries(FocusPro PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system
)

# WinMain error එක අයින් කරන Linker flag එක
set_target_properties(FocusPro PROPERTIES 
    LINK_FLAGS "/ENTRY:mainCRTStartup"
)