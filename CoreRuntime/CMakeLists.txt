# CMakeLists.txt for CoreRuntime library

set(WINDOWS_LIBS dxgi.lib d3dcompiler.lib d3d11.lib $<$<CONFIG:DEBUG>:dxguid.lib>)
set(LIBS $<$<PLATFORM_ID:Windows>:${WINDOWS_LIBS}>)

set(DEFINES
    QUILL_DISABLE_NON_PREFIXED_MACROS
)

if (WIN32)
    find_package(wil CONFIG REQUIRED)
    find_package(directxtk CONFIG REQUIRED)
endif ()

find_package(quill CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(EASTL CONFIG REQUIRED)

find_path(RAPIDHASH_INCLUDE_DIRS "rapidhash.h")
find_path(GTL_INCLUDE_DIRS "gtl/adv_utils.hpp")

add_library(CoreRuntime STATIC
    Include/ByteEngine/Core/Application.h
    Include/ByteEngine/Core/MainWindow.h
    Include/ByteEngine/Core/Singleton.h
    Include/ByteEngine/Core/EventSystem/Delegate.h
    Include/ByteEngine/Core/EventSystem/MulticastDelegate.h
    Include/ByteEngine/Core/Input/Input.h
    Include/ByteEngine/Core/Input/KeyCode.h
    Include/ByteEngine/Details/Core/EventSystem/Subscriptions.h
    Include/ByteEngine/Math/Math.h
    Include/ByteEngine/Math/Quaternion.h
    Include/ByteEngine/Math/Vector2.h
    Include/ByteEngine/Math/Vector3.h
    Include/ByteEngine/Math/Vector4.h
    Include/ByteEngine/Utilities/EnumFlagsOperators.h
    Include/ByteEngine/WinApiExcludingDefs/LeanAndMean.h
    Include/ByteEngine/WinApiExcludingDefs/NoAll.h
    Include/ByteEngine/WinApiExcludingDefs/NoGdi.h
    Include/ByteEngine/WinApiExcludingDefs/NoInput.h
    Include/ByteEngine/WinApiExcludingDefs/NoIO.h
    Include/ByteEngine/WinApiExcludingDefs/NoLocalization.h
    Include/ByteEngine/WinApiExcludingDefs/NoSys.h
    Include/ByteEngine/WinApiExcludingDefs/NoUiAndWindows.h
    Include/ByteEngine/Debug.h
    Include/ByteEngine/GameTime.h
    Source/Core/Application.cpp
    Source/Core/Input/Input.cpp
    Source/Math/Math.cpp
    Include/ByteEngine/Math/Matrix4x4.h
    Source/Core/Graphics/GraphicsDevice.h
    Include/ByteEngine/Math/Color.h
    Source/Platform/Core/Graphics/GraphicsDeviceD3D11.cpp
    Source/Platform/Core/Graphics/GraphicsDeviceD3D11.h
    Include/ByteEngine/Utilities/Utils.h
    Source/Debug.cpp
    Include/ByteEngine/Core/Memory/Allocator.h
    Source/Core/Memory/StdAllocator.cpp
    Source/Core/Memory/NewDeleteOverrides.cpp
    Include/ByteEngine/Details/Core/Memory/CustomContainerAllocator.h
    Include/ByteEngine/Containers/HashMap.h
    Include/ByteEngine/Details/Containers/Hasher.h
    Include/ByteEngine/Containers/ConcurrentHashMap.h
    Include/ByteEngine/Utilities/HashUtils.h
    Include/ByteEngine/String/StringName.h
    Include/ByteEngine/String/String.h
    Include/ByteEngine/Details/Core/Memory/CustomEastlAllocator.h
    Source/Details/CustomEastlAllocator.cpp
    Include/ByteEngine/Core/Memory/SmartPointers.h
    Source/String/StringName.cpp
)

target_link_libraries(CoreRuntime PRIVATE project_options ${LIBS})
target_link_libraries(CoreRuntime PUBLIC quill::quill fmt::fmt EASTL)

target_include_directories(CoreRuntime PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include" ${RAPIDHASH_INCLUDE_DIRS} ${GTL_INCLUDE_DIRS})
target_include_directories(CoreRuntime PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")

target_compile_definitions(CoreRuntime PRIVATE $<$<PLATFORM_ID:Windows>:UNICODE;_UNICODE>)
target_compile_definitions(CoreRuntime PUBLIC ${DEFINES})

if (WIN32)
    target_link_libraries(CoreRuntime PRIVATE WIL::WIL Microsoft::DirectXTK)
endif ()