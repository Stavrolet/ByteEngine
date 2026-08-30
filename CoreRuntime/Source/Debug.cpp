#include "ByteEngine/Debug.h"

#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/sinks/ConsoleSink.h>
#ifdef BE_WINDOWS
    #include <Windows.h>
#endif

using namespace quill;

namespace ByteEngine
{
    bool Debug::IsDebuggerAttached()
    {
#ifdef BE_WINDOWS
        return IsDebuggerPresent();
#endif
    }

    void Debug::Initialize()
    {
        BackendOptions backendOptions;
        backendOptions.check_printable_char = { };
        Backend::start(backendOptions);

        logger = Frontend::create_or_get_logger(
            "Main",
            Frontend::create_or_get_sink<ConsoleSink>("ConsoleSink1"),
            PatternFormatterOptions { "[%(time)] [THREAD:%(thread_name)] %(level): \"%(message)\" at %(source_location)" });
    }

    void Debug::Shutdown()
    {
        Backend::stop();
    }
} // namespace ByteEngine