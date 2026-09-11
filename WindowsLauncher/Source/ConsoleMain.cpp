#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

int main()
{
    const LPSTR rawCmdLine = GetCommandLine();

    bool inQuotes = false;
    const char* ptr = rawCmdLine;
    while (*ptr)
    {
        if (*ptr == '"')
            inQuotes = !inQuotes;
        else if (!inQuotes && *ptr == ' ')
            break;
        ptr++;
    }

    while (*ptr == ' ')
        ptr++;

    std::string newCmdLine = "\"\"";
    newCmdLine.insert(1, BE_GUI_EXE_NAME);
    if (*ptr != '\0')
    {
        newCmdLine += " ";
        newCmdLine += ptr;
    }

    std::vector<char> cmdLineBuffer(newCmdLine.begin(), newCmdLine.end());
    cmdLineBuffer.push_back('\0');

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    const bool success = CreateProcess(
        BE_GUI_EXE_NAME,
        cmdLineBuffer.data(),
        nullptr,
        nullptr,
        TRUE,
        0,
        nullptr,
        nullptr,
        &si,
        &pi
    );

    if (success)
    {
        WaitForSingleObject(pi.hProcess, INFINITE);

        DWORD exitCode = 0;
        GetExitCodeProcess(pi.hProcess, &exitCode);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return exitCode;
    }
    else
    {
        LPSTR messageBuffer = nullptr;

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&messageBuffer),
            0,
            nullptr
        );

        std::cerr << "Failed to create process. Error code: " << messageBuffer << std::endl;
        return -1;
    }
}