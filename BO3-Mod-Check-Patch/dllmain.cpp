#include <Windows.h>
#include <iomanip>
#include <limits>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "memory.h"
#include <thread>

std::vector<std::thread> threads;

#ifdef max
#undef max
#endif

void ShowMessageBox(const char* message) { 
    const char* title = "BO3 Mod Check Patch";

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, message, -1, NULL, 0);
    int size_title_needed = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);

    std::wstring wMessage(size_needed, L'\0');
    std::wstring wTitle(size_title_needed, L'\0');

    MultiByteToWideChar(CP_UTF8, 0, message, -1, &wMessage[0], size_needed);
    MultiByteToWideChar(CP_UTF8, 0, title, -1, &wTitle[0], size_title_needed);

    MessageBoxW(NULL, wMessage.c_str(), wTitle.c_str(), MB_OK);
}


// The rank parts of the game are asking if a mod is loaded
// So we run the og func (some mods need it) and return false anyway
bool isModLoadedRedirect() 
{ 
	return false;
}


DWORD WINAPI attach(_In_ LPVOID lpParameter)
{ 
	bmem::init(nullptr);

    // September 2026 Update Offsets
    uintptr_t isModLoadedFunc1 = 0x1420C9420_offset;
    bmem::createJump(isModLoadedFunc1, (uintptr_t)isModLoadedRedirect);

    // These offsets are calls to a func which returns if a mod is loaded or not
    // We replace it with our own func to tell it theres no mods
    const std::vector<uintptr_t> isModLoadedAddrs =
    {
        { 0x1420EAD04_offset },
        { 0x1420EAD24_offset },
        { 0x1420EAC64_offset },
        { 0x1420EAC34_offset },
        { 0x1420EA6D7_offset },
        { 0x1413E6A74_offset },
        { 0x1415E7EDB_offset },
        { 0x1415E87DB_offset },
        { 0x1415EBAE9_offset },
        { 0x1415F1F29_offset },
        { 0x1415F1FD9_offset },
        { 0x1415F20A0_offset },
        { 0x1415F7F60_offset },
        { 0x141A80D1D_offset },
        { 0x141A9AD29_offset },
        { 0x141E93C6B_offset },
        { 0x141E9C1EE_offset },
        { 0x141EA4FA9_offset },
        { 0x141EBF00D_offset },
        { 0x1420EA69D_offset },
    };

	for (uintptr_t address : isModLoadedAddrs)
	{
		bmem::createCall(address, (uintptr_t)isModLoadedRedirect);
	}

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) { 
    if (fdwReason == DLL_PROCESS_ATTACH)
    { 
        CreateThread(NULL, NULL, attach, NULL, NULL, NULL);
    }
    else if (fdwReason == DLL_PROCESS_DETACH)
    { 

    }

    return true;
}