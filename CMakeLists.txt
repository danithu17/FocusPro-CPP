cmake_minimum_required(VERSION 3.15)
project(FocusProUI)

set(CMAKE_CXX_STANDARD 17)

# NuGet packages thiyena thana CMake ekata kiyamu
set(SFML_DIR "${CMAKE_SOURCE_DIR}/packages/sfml_graphics.2.6.0/build/native")

# Libraries tika manually hoyanna kiyamu (Safe method)
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

add_executable(FocusPro main.cpp)
target_link_libraries(FocusPro PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

if(MSVC)
    set_target_properties(FocusPro PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()