#include <windows.h>
#include <tlhelp32.h>
#include <wininet.h>
#include <iostream>
#include <string>
#include <regex>
#include <algorithm>

#pragma comment(lib, "wininet.lib")

HANDLE prochandle = nullptr;
DWORD procid = 0;
uintptr_t base = 0;

std::string HttpGet(const std::string& url) {
    HINTERNET hInternet = InternetOpenA("RobloxESP", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    if (!hInternet) return "";

    HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), nullptr, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return "";
    }

    std::string result;
    char buffer[4096];
    DWORD bytesRead;

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        result.append(buffer, bytesRead);
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return result;
}

uintptr_t fo() {
    try {
        std::string response = HttpGet("https://p.metixud.xyz/fflags.php");
        if (response.empty()) {
            return 0x7936d08; // update this if my host is down.
        }

        std::regex pattern(R"(inline constexpr FFlag DebugDrawBroadPhaseAABBs\s*=\s*(0x[0-9a-fA-F]+))");
        std::smatch match;

        if (std::regex_search(response, match, pattern)) {
            return std::stoull(match[1].str(), nullptr, 16);
        }
        return 0x7936d08; // update this if my host is down.
    }
    catch (...) {
        return 0x7936d08; // update this if my host is down.
    }
}

bool frp() {
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) return false;

    if (Process32First(hProcessSnap, &pe32)) {
        do {
            if (wcscmp(pe32.szExeFile, L"RobloxPlayerBeta.exe") == 0) {
                procid = pe32.th32ProcessID;
                CloseHandle(hProcessSnap);
                return true;
            }
        } while (Process32Next(hProcessSnap, &pe32));
    }

    CloseHandle(hProcessSnap);
    return false;
}

bool opr() {
    prochandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, procid);
    return prochandle != nullptr;
}

bool gba() {
    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);

    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procid);
    if (hModuleSnap == INVALID_HANDLE_VALUE) return false;

    if (Module32First(hModuleSnap, &me32)) {
        do {
            if (wcscmp(me32.szModule, L"RobloxPlayerBeta.exe") == 0) {
                base = reinterpret_cast<uintptr_t>(me32.modBaseAddr);
                CloseHandle(hModuleSnap);
                return true;
            }
        } while (Module32Next(hModuleSnap, &me32));
    }

    CloseHandle(hModuleSnap);
    return false;
}

bool WriteMemory(uintptr_t address, bool value) {
    BYTE byteValue = value ? 1 : 0;
    SIZE_T bytesWritten;
    return WriteProcessMemory(prochandle, reinterpret_cast<LPVOID>(address), &byteValue, sizeof(byteValue), &bytesWritten);
}

int main() {
    if (!frp()) {
        std::cout << "roblox not found\n";
        return 0;
    }

    if (!opr()) {
        std::cout << "failed to open process\n";
        return 0;
    }

    if (!gba()) {
        std::cout << "failed to get base\n";
        return 0;
    }

    uintptr_t offset = fo();
    uintptr_t address = base + offset;

    std::string choice;
    while (true) {
        std::cout << "do u wanna have AABBs: ";
        std::getline(std::cin, choice);
        std::transform(choice.begin(), choice.end(), choice.begin(), ::tolower);

        if (choice == "yes") {
            WriteMemory(address, true);
            std::cout << "AABBs enabled\n";
        }
        else if (choice == "no") {
            WriteMemory(address, false);
            std::cout << "AABBs disabled\n";
        }
        else if (choice == "exit") {
            break;
        }
    }

    if (prochandle) CloseHandle(prochandle);
    return 0;

}
