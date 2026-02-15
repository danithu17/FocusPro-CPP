cmake_minimum_required(VERSION 3.15)
project(FocusPro-CPP)

# C++ 20 Standard එක පාවිච්චි කරනවා
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static linking සඳහා MSVC runtime එක නිවැරදිව set කරනවා (MultiThreaded)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# -------------------------------------------------------------------
# SFML 3.0 සොයා ගැනීම
# -------------------------------------------------------------------
# මෙතැනදී COMPONENTS කෑල්ල අයින් කළේ vcpkg එකේ එන "Unsupported component" 
# කියන error එක bypass කරන්නයි.
find_package(SFML 3.0 REQUIRED)

# -------------------------------------------------------------------
# Executable එක නිර්මාණය කිරීම
# -------------------------------------------------------------------
# WIN32 යෙදීමෙන් CMD window එක ඉවත් කෙරේ.
add_executable(FocusPro WIN32 main.cpp)

# -------------------------------------------------------------------
# Libraries ලින්ක් කිරීම
# -------------------------------------------------------------------
# SFML 3.0 වලදී targets සැමවිටම Capital Letters වලින් ආරම්භ වේ (Graphics, Window, System).
# PRIVATE keyword එක භාවිතා කරලා vcpkg ලින්කින් ප්‍රශ්න මගහරවා ගන්නවා.
target_link_libraries(FocusPro PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

# -------------------------------------------------------------------
# Windows Specific Settings
# -------------------------------------------------------------------
# WIN32 flag එක දැම්මම එන Linker error එක (WinMain error) මගහැරීමට,
# entry point එක සාමාන්‍ය main එකට set කරනවා.
set_target_properties(FocusPro PROPERTIES 
    LINK_FLAGS "/ENTRY:mainCRTStartup"
)