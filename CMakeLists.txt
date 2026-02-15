cmake_minimum_required(VERSION 3.15)
project(FocusPro-CPP)

# C++ 20 standard එක පාවිච්චි කරනවා
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static linking කරන නිසා MSVC runtime එක නිවැරදිව set කරනවා
# මේකෙන් build එකේ errors එන එක වළකිනවා
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# SFML 3.0 සොයා ගැනීම (graphics, window, system components)
find_package(SFML 3.0 REQUIRED COMPONENTS graphics window system)

# WIN32 කියන එක දැම්මම තමයි අර කළු පාට CMD window එක නැති වෙන්නේ
add_executable(FocusPro WIN32 main.cpp)

# SFML libraries ලින්ක් කිරීම (PRIVATE keyword එක අනිවාර්යයි)
# SFML 3.0 වලදී SFML:: භාවිතා කිරීම වඩාත් නිවැරදි ක්‍රමයයි
target_link_libraries(FocusPro PRIVATE 
    SFML::graphics 
    SFML::window 
    SFML::system
)

# WIN32 flag එක දැම්මම එන Linker error එක (WinMain error) 
# අයින් කරන්න මේ properties ටික අනිවාර්යයෙන්ම ඕනෑ
set_target_properties(FocusPro PROPERTIES 
    LINK_FLAGS "/ENTRY:mainCRTStartup"
)