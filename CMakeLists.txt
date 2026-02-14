cmake_minimum_required(VERSION 3.15)
project(FocusProUI)

set(CMAKE_CXX_STANDARD 17)

# SFML සොයා ගැනීමට පෙර vcpkg path එක තහවුරු කරමු
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# CONFIG mode එකෙන් හරියටම SFML components ටික ඉල්ලමු
find_package(SFML CONFIG REQUIRED COMPONENTS graphics window system audio)

add_executable(FocusPro main.cpp)

# SFML libraries හරියටම link කරමු
target_link_libraries(FocusPro PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

# Windows වල Console එක hide කරන්න
if(MSVC)
    set_target_properties(FocusPro PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()