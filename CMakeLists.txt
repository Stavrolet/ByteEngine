# Root CMakeLists.txt

cmake_minimum_required(VERSION 3.20)
unset(CMAKE_INCLUDE_SYSTEM_FLAG_CXX)
unset(_CMAKE_INCLUDE_SYSTEM_FLAG_CXX_WARNING)

project(ByteEngineD3D11 CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_SOURCE_DIR}/GlobalBuildConfigs.cmake)

enable_testing()

add_subdirectory(CoreRuntime)
add_subdirectory(WindowsLauncher)
add_subdirectory(Tests)
add_subdirectory(MyLocalTests)