# CMakeLists.txt for CoreRuntime library

set(WINDOWS_LIBS dxgi.lib d3dcompiler.lib d3d11.lib $<$<CONFIG:DEBUG>:dxguid.lib>)
set(LIBS $<$<PLATFORM_ID:Windows>:${WINDOWS_LIBS}>)

if(WIN32)
	find_package(wil CONFIG REQUIRED)
	find_package(directxtk CONFIG REQUIRED)
endif()

find_package(quill CONFIG REQUIRED)

add_library(CoreRuntime STATIC 
	"Include/ByteEngine/Core/Base/Application.h"
	"Include/ByteEngine/Core/Base/MainWindow.h"
	"Include/ByteEngine/Core/Base/Singleton.h"
	"Include/ByteEngine/Core/EventSystem/Delegate.h"
	"Include/ByteEngine/Core/EventSystem/MulticastDelegate.h"
	"Include/ByteEngine/Core/Input/Input.h"
	"Include/ByteEngine/Core/Input/KeyCode.h"
	"Include/ByteEngine/Detail/Core/EventSystem/Subscriptions.h"	
	"Include/ByteEngine/Math/Math.h"
	"Include/ByteEngine/Math/Quaternion.h"
	"Include/ByteEngine/Math/Vector2.h"
	"Include/ByteEngine/Math/Vector3.h"
	"Include/ByteEngine/Math/Vector4.h"
	"Include/ByteEngine/Utilities/BitFlagsHelper.h"
	"Include/ByteEngine/Utilities/EnumFlagsOperators.h"
	"Include/ByteEngine/WinApiExcludingDefs/LeanAndMean.h"
	"Include/ByteEngine/WinApiExcludingDefs/NoAll.h"
	"Include/ByteEngine/WinApiExcludingDefs/NoGdi.h"
	"Include/ByteEngine/WinApiExcludingDefs/NoInput.h"
	"Include/ByteEngine/WinApiExcludingDefs/NoIO.h"
	"Include/ByteEngine/WinApiExcludingDefs/NoLocalization.h"
	"Include/ByteEngine/WinApiExcludingDefs/NoSys.h"
	"Include/ByteEngine/WinApiExcludingDefs/NoUiAndWindows.h"	
	"Include/ByteEngine/Debug.h"
	"Include/ByteEngine/GameTime.h"
	"Include/ByteEngine/Primitives.h"
	"Source/Core/Base/Application.cpp"
	"Source/Core/Input/Input.cpp"
	"Source/Math/Math.cpp"
	"Source/Math/Quaternion.cpp"	
 	"Include/ByteEngine/Math/Matrix4x4F.h"
	"Source/Math/Matrix4x4F.cpp"
	"Source/Core/Graphics/GraphicsDevice.h"
	"Include/ByteEngine/Math/Rotation.h" 
	"Source/Math/Rotation.cpp" 
	"Include/ByteEngine/Math/Color.h" 
	"Source/Platform/Core/Graphics/GraphicsDeviceD3D11.cpp" 
	"Source/Platform/Core/Graphics/GraphicsDeviceD3D11.h" 
	"Include/ByteEngine/Utilities/Utils.h" 
	"Source/Debug.cpp"
)

target_link_libraries(CoreRuntime PRIVATE project_options ${LIBS})
target_link_libraries(CoreRuntime PUBLIC quill::quill)
target_include_directories(CoreRuntime PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")
target_include_directories(CoreRuntime PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")
target_compile_definitions(CoreRuntime PRIVATE $<$<PLATFORM_ID:Windows>:UNICODE;_UNICODE> QUILL_DISABLE_NON_PREFIXED_MACROS)

if(WIN32)
	target_link_libraries(CoreRuntime PRIVATE WIL::WIL Microsoft::DirectXTK)
endif()