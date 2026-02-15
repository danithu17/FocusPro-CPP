cmake_minimum_required(VERSION 3.15)
project(FocusPro-CPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static build එකට MSVC settings
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# SFML 3.0 සොයා ගැනීම
find_package(SFML 3.0 REQUIRED)

# Executable නිර්මාණය (WIN32 යෙදීමෙන් CMD එක ඉවත් වේ)
add_executable(FocusPro WIN32 main.cpp)

# SFML libraries ලින්ක් කිරීම
target_link_libraries(FocusPro PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

# Entry point එක fix කිරීම (WinMain error එක මගහැරීමට)
set_target_properties(FocusPro PROPERTIES 
    LINK_FLAGS "/ENTRY:mainCRTStartup"
)