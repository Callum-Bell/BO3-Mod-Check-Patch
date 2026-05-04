// Custom memory editing functions

#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <algorithm>

//#pragma comment(lib, "../Lib/minhook.x64.lib")
#include "MinHook.h"

namespace bmem
{
    struct pattern_byte
    {
        pattern_byte() : ignore(true) {}

        explicit pattern_byte(const std::string& byte_string, const bool ignore_this = false) : ignore(ignore_this),
            data(string_to_uint8(byte_string)) {}

        bool ignore;
        uint8_t data;

    private:
        static uint8_t string_to_uint8(const std::string& str)
        {
            std::istringstream iss(str);
            uint32_t ret;

            if (iss >> std::hex >> ret) return static_cast<uint8_t>(ret);

            return 0;
        }
    };

    static bool initalized = false;
    static uintptr_t game_base;
    static uintptr_t image_size;

    static void init(const char* moduleName)
    {
        if (initalized) { return; }

        if (moduleName == "currentproc") { moduleName = nullptr; }

        game_base = reinterpret_cast<uintptr_t>(GetModuleHandleA(moduleName));
        const auto header = reinterpret_cast<const IMAGE_DOS_HEADER*>(game_base);
        const auto nt_header = reinterpret_cast<const IMAGE_NT_HEADERS64*>(reinterpret_cast<const uint8_t*>(header) + header->e_lfanew);
        image_size = nt_header->OptionalHeader.SizeOfImage;

        MH_Initialize();
    }

    static uintptr_t getGameBase(const char* moduleName = "currentproc")
    {
        init(moduleName);

        return game_base;
    }

    static uintptr_t getAddressFromPattern(const std::string pattern, const char* moduleName = "currentproc")
    {
        init(moduleName);

        std::vector<pattern_byte> p;
        std::istringstream iss(pattern);
        std::string w;

        while (iss >> w)
        {
            if (w[0] == '?')
            {
                // Wildcard
                p.emplace_back();
            }
            else if (w.length() == 2 && isxdigit(w[0]) && isxdigit(w[1]))
            {
                // Hex
                p.emplace_back(w);
            }
            else  return NULL;
        }

        for (uintptr_t i = 0; i < image_size; i++)
        {
            auto current_byte = reinterpret_cast<uint8_t*>(game_base + i);
            auto found = true;

            for (size_t ps = 0; ps < p.size(); ps++)
            {
                if (p[ps].ignore == false && current_byte[ps] != p[ps].data)
                {
                    found = false;
                    break;
                }
            }

            if (found) return reinterpret_cast<uintptr_t>(current_byte);
        }

        return NULL;
    }

    static std::vector<uintptr_t> getAllAddressesFromPattern(const std::string pattern, const char* moduleName = "currentproc")
    {
        init(moduleName);

        std::vector<pattern_byte> p;
        std::istringstream iss(pattern);
        std::string w;

        while (iss >> w)
        {
            if (w[0] == '?')
            {
                // Wildcard
                p.emplace_back();
            }
            else if (w.length() == 2 && isxdigit(w[0]) && isxdigit(w[1]))
            {
                // Hex
                p.emplace_back(w);
            }
            else return {};  // Return empty if pattern is invalid
        }

        std::vector<uintptr_t> results;

        for (uintptr_t i = 0; i < image_size; i++)
        {
            auto current_byte = reinterpret_cast<uint8_t*>(game_base + i);
            bool found = true;

            for (size_t ps = 0; ps < p.size(); ps++)
            {
                if (!p[ps].ignore && current_byte[ps] != p[ps].data)
                {
                    found = false;
                    break;
                }
            }

            if (found) results.push_back(reinterpret_cast<uintptr_t>(current_byte));
        }

        return results;
    }

    template <typename T>
    static constexpr auto relativeToAbsolute(uintptr_t address, int addressOffset, int instructionCount) noexcept
    {
        return (T)(address + instructionCount + *reinterpret_cast<std::int32_t*>(address + addressOffset));
    }

    static uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
    {
        uintptr_t addr = ptr;
        for (unsigned int i = 0; i < offsets.size(); ++i)
        {
            addr = *(uintptr_t*)addr;
            addr += offsets[i];
        }
        return addr;
    }


    static void createJump(uintptr_t jumpAtAddress, uintptr_t jumpToAddress)
    {
        // Minhook is for "hooking" but in short it basically makes a jump to a address
        // with the added feature of auto making trampolines
        MH_CreateHook((LPVOID)jumpAtAddress, (LPVOID)jumpToAddress, nullptr);
        MH_EnableHook((LPVOID)jumpAtAddress);
    }

    static void createCall(uintptr_t callAtAddress, uintptr_t addressToCall)
    {
        // Create a jump since minhook auto supports trampolines if needed
        createJump(callAtAddress, addressToCall);

        // Change the jump to a call
        DWORD old_protect;
        VirtualProtect(reinterpret_cast<void*>(callAtAddress), 1, PAGE_EXECUTE_READWRITE, &old_protect);
        *reinterpret_cast<uint8_t*>(callAtAddress) = 0xE8; // E8 = call;
        VirtualProtect(reinterpret_cast<void*>(callAtAddress), 1, old_protect, &old_protect);
    }

    // Encodes a address into a custom offset
    static inline size_t derelocate(const size_t val)
    {
        if (!val) return 0;

        return (val - bmem::getGameBase()) + 0x140000000;
    }
}

// Decodes a custom offset into a full address
// Example Useage: 0x1420F7484_offset
static inline size_t operator"" _offset(const size_t val)
{
    if (!val) return 0;

    return bmem::getGameBase() + (val - 0x140000000);
}