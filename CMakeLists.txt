cmake_minimum_required(VERSION 3.10)
project(FocusPro)

set(CMAKE_CXX_STANDARD 17)

# Create the executable
add_executable(FocusPro main.cpp)

# Link Windows Native Libraries
target_link_libraries(FocusPro PRIVATE User32.lib Gdi32.lib Shell32.lib)

# Set it as a Windows GUI app (No console)
set_target_properties(FocusPro PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")