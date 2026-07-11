set(DEBUG_COMPILER_FLAGS $<$<CXX_COMPILER_ID:MSVC>:/JMC;/RTC1>)
set(RELEASE_COMPILER_FLAGS $<$<CXX_COMPILER_ID:MSVC>:/O2;/Oi;/GL;/Gy;/Zi>)

set(COMMON_COMPILER_FLAGS $<$<CXX_COMPILER_ID:MSVC>:
    /utf-8;
    /sdl;
    /W2;/WX-;
    /diagnostics:column;/GS;
    /Zc:wchar_t;/Zc:forScope;/Zc:inline;
    /Gd;/nologo;/FC;
    /errorReport:prompt;
    /fp:fast;/arch:AVX2>
)

set(WINDOWS_DEFINES 
    _WINDOWS
    WIN32
    _HAS_EXCEPTIONS=0
    BE_WINDOWS
)

set(COMMON_DEFINES $<$<PLATFORM_ID:Windows>:${WINDOWS_DEFINES}>)

set(DEBUG_LINKER_FLAGS  )
set(RELEASE_LINKER_FLAGS $<$<CXX_COMPILER_ID:MSVC>:/OPT:REF;/OPT:ICF;/LTCG:incremental>)
set(COMMON_LINKER_FLAGS $<$<CXX_COMPILER_ID:MSVC>:/NOLOGO;/DYNAMICBASE;/NXCOMPAT;/ERRORREPORT:PROMPT>)

add_library(project_options INTERFACE)

target_compile_options(project_options INTERFACE
    $<$<CONFIG:Debug>:${DEBUG_COMPILER_FLAGS}>
    $<$<CONFIG:Release>:${RELEASE_COMPILER_FLAGS}>
    ${COMMON_COMPILER_FLAGS}
)

target_compile_definitions(project_options INTERFACE
    $<$<CONFIG:Debug>:_DEBUG;BE_DEBUG>
    $<$<CONFIG:Release>:NDEBUG;BE_RELEASE>
    ${COMMON_DEFINES}
)

target_link_options(project_options INTERFACE
    $<$<CONFIG:Debug>:${DEBUG_LINKER_FLAGS}>
    $<$<CONFIG:Release>:${RELEASE_LINKER_FLAGS}>
    ${COMMON_LINKER_FLAGS}
)