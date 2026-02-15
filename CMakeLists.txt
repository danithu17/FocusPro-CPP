cmake_minimum_required(VERSION 3.15)
project(FocusPro-CPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static Runtime එක අනිවාර්යයි
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# 1. Component ලැයිස්තුව නැතුව SFML හොයන්න (Error එක වළක්වයි)
find_package(SFML 3.0 REQUIRED)

# 2. Executable එක හදන්න (WIN32 මගින් CMD window එක ඉවත් කරයි)
add_executable(FocusPro WIN32 main.cpp)

# 3. vcpkg static build එකකදී අතින් libraries ලින්ක් කිරීම
# SFML 3.0 වල static targets මේවායි:
target_link_libraries(FocusPro PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
    opengl32
    winmm
    gdi32
)

# 4. Entry point එක fix කිරීම
set_target_properties(FocusPro PROPERTIES 
    LINK_FLAGS "/ENTRY:mainCRTStartup"
)