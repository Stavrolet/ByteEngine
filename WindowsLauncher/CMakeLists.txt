# CMakeLists.txt for Launcher executable

add_executable(Launcher
    "Source/Main.cpp"
    "Source/Win32Window.cpp"
    "Source/Win32Window.h"
)

target_link_libraries(Launcher PRIVATE project_options CoreRuntime)

target_include_directories(Launcher PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Resources" "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set_target_properties(Launcher PROPERTIES WIN32_EXECUTABLE TRUE)