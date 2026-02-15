cmake_minimum_required(VERSION 3.15)
project(FocusPro-CPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static build එකට අනිවාර්ය MSVC settings
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# SFML 3.0 නිවැරදිව සොයා ගැනීම (lowercase components වැදගත්)
find_package(SFML 3.0 REQUIRED COMPONENTS graphics window system)

# Executable එක (WIN32 මගින් CMD window එක ඉවත් කරයි)
add_executable(FocusPro WIN32 main.cpp)

# Libraries ලින්ක් කිරීම
target_link_libraries(FocusPro PRIVATE 
    SFML::graphics 
    SFML::window 
    SFML::system
)

# Entry point එක fix කිරීම (WinMain error එක සඳහා)
set_target_properties(FocusPro PROPERTIES 
    LINK_FLAGS "/ENTRY:mainCRTStartup"
)