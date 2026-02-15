cmake_minimum_required(VERSION 3.15)
project(FocusPro-CPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static build එකට අනිවාර්ය MSVC settings
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# 1. SFML package එක විතරක් load කරන්න
find_package(SFML 3.0 REQUIRED COMPONENTS graphics window system)

# 2. Executable එක හදන්න (WIN32 නිසා CMD එක අයින් වෙනවා)
add_executable(FocusPro WIN32 main.cpp)

# 3. Targets manually link කිරීම (SFML 3.0 vcpkg standard)
# මෙතැනදී SFML::graphics වැනි namespaces පාවිච්චි කරමු
target_link_libraries(FocusPro PRIVATE 
    SFML::graphics 
    SFML::window 
    SFML::system
)

# 4. WinMain entry point error එක මගහැරීමට
set_target_properties(FocusPro PROPERTIES 
    LINK_FLAGS "/ENTRY:mainCRTStartup"
)