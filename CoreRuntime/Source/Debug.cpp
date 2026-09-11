#include "ByteEngine/Debug.h"

#include <iostream>
#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/sinks/ConsoleSink.h>
#ifdef BE_WINDOWS
    #include <Windows.h>
#endif

using namespace quill;

namespace ByteEngine
{
    void Debug::BreakpointIfDebuggerAttached()
    {
        if (IsDebuggerAttached())
            Breakpoint();
    }

    bool Debug::IsDebuggerAttached()
    {
#ifdef BE_WINDOWS
        return IsDebuggerPresent();
#endif
    }

    void Debug::Initialize()
    {
#ifdef BE_WINDOWS
        if (AttachConsole(ATTACH_PARENT_PROCESS))
        {
            FILE* dummy;
            freopen_s(&dummy, "CONOUT$", "w", stdout);
            freopen_s(&dummy, "CONOUT$", "w", stderr);
            freopen_s(&dummy, "CONIN$", "r", stdin);

            std::cout.clear();
            std::cerr.clear();
            std::clog.clear();
            std::cin.clear();

            std::cout << '\r';
        }
#endif

        BackendOptions backendOptions;
        backendOptions.check_printable_char = { };
        Backend::start(backendOptions);

        PatternFormatterOptions patternFormatterOptions;
        patternFormatterOptions.format_pattern = "[%(time)] [THREAD:%(thread_name)] %(log_level) \"%(source_location)\": %(message)";
        patternFormatterOptions.add_metadata_to_multi_line_logs = false;

        logger = Frontend::create_or_get_logger("Main", Frontend::create_or_get_sink<ConsoleSink>("ConsoleSink1"), patternFormatterOptions);
    }

    void Debug::Shutdown()
    {
        Backend::stop();
    }
} // namespace ByteEngine