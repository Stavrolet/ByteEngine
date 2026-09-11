# CMakeLists.txt for Launcher executable

add_executable(Launcher
    "Source/Main.cpp"
    "Source/Win32Window.cpp"
    "Source/Win32Window.h"
)

target_link_libraries(Launcher PRIVATE project_options CoreRuntime)

target_include_directories(Launcher PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Resources" "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set_target_properties(Launcher PROPERTIES WIN32_EXECUTABLE TRUE)

set(CMAKE_EXECUTABLE_SUFFIX_C "")
set(CMAKE_EXECUTABLE_SUFFIX_CXX "")
set(CMAKE_EXECUTABLE_SUFFIX "")

add_executable(LauncherConsole
    "Source/ConsoleMain.cpp"
)

set_target_properties(LauncherConsole PROPERTIES
    OUTPUT_NAME "$<TARGET_FILE_NAME:Launcher>"
    SUFFIX ".com"
)

target_link_libraries(LauncherConsole PRIVATE project_options)
target_compile_definitions(LauncherConsole PRIVATE BE_GUI_EXE_NAME="$<TARGET_FILE_NAME:Launcher>")
set_target_properties(LauncherConsole PROPERTIES WIN32_EXECUTABLE FALSE)

add_dependencies(LauncherConsole Launcher)