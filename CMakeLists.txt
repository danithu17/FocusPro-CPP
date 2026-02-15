cmake_minimum_required(VERSION 3.15)
project(FocusPro-CPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static Runtime config
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# 1. SFML package එක විතරක් හොයන්න (components නැතුව)
find_package(SFML 3.0 REQUIRED)

# 2. Executable එක හදන්න (WIN32 නිසා CMD window එක එන්නේ නැහැ)
add_executable(FocusPro WIN32 main.cpp)

# 3. SFML වල අභ්‍යන්තර targets කෙලින්ම ලින්ක් කිරීම
# SFML 3.0 වල vcpkg මගින් සපයන නිවැරදි targets මේවායි
target_link_libraries(FocusPro PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system
    opengl32
)

# 4. Entry point එක fix කිරීම (WinMain error එක මගහැරීමට)
set_target_properties(FocusPro PROPERTIES 
    LINK_FLAGS "/ENTRY:mainCRTStartup"
)