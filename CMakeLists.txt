cmake_minimum_required(VERSION 3.10)
project(FocusPro)
set(CMAKE_CXX_STANDARD 17)
add_executable(FocusPro main.cpp)
# Listview control eka weda karanna Comctl32 link karanna ona
target_link_libraries(FocusPro PRIVATE User32.lib Gdi32.lib Shell32.lib Comctl32.lib)
set_target_properties(FocusPro PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")