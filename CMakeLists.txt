cmake_minimum_required(VERSION 3.15)
project(FocusPro-CPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static build එකට අදාළ MSVC settings
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# 1. Component ලැයිස්තුව නැතුව SFML හොයන්න (Error එක එන්නේ මෙතැනදීයි)
find_package(SFML 3.0 REQUIRED)

# 2. Executable එක හදන්න (WIN32 නිසා CMD එක එන්නේ නැහැ)
add_executable(FocusPro WIN32 main.cpp)

# 3. Targets manually link කිරීම (vcpkg වල SFML 3.0 සඳහා නිවැරදි targets)
target_link_libraries(FocusPro PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

# 4. Entry point එක fix කිරීම (WinMain error එක සඳහා)
set_target_properties(FocusPro PROPERTIES 
    LINK_FLAGS "/ENTRY:mainCRTStartup"
)