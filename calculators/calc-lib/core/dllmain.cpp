#include <Windows.h>

int __stdcall DllMain(HMODULE Module, DWORD Reason, LPVOID Reserved)
{
    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

