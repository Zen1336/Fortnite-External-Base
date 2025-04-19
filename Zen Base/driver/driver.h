#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

#define REQUEST_READWRITE CTL_CODE(FILE_DEVICE_UNKNOWN, 0xA337, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define REQUEST_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1A37, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define REQUEST_CR3 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x13A7, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define REQUEST_BSOD CTL_CODE(FILE_DEVICE_UNKNOWN, 0x133A, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define REQUEST_SECURITY 0xD84AB45

typedef struct _ReadWrite {
    INT32 SECURITY;
    INT32 PID;
    ULONGLONG ADDRESS;
    ULONGLONG BUFFER;
    ULONGLONG SIZE;
    BOOLEAN WRITE;
} ReadWrite, * PReadWrite;

typedef struct _Base {
    INT32 SECURITY;
    INT32 PID;
    ULONGLONG* BASE;
} niggerBase, * PBase;

typedef struct _CR3 {
    INT32 SECURITY;
    uint32_t PID;
    ULONGLONG* CR3;
} CR3, * PCR3;

namespace driver
{
    inline HANDLE hDriver;
    inline INT32 PID;

    inline bool Init() {
        hDriver = CreateFileW(L"\\\\.\\{Zeus-Drvmoment}", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        if (!hDriver || (hDriver == INVALID_HANDLE_VALUE))
            return false;
        return true;
    }

    inline void ReadPhsyical(PVOID ADDY, PVOID PUF, DWORD SIZE) {
        _ReadWrite Request = { REQUEST_SECURITY, PID, (ULONGLONG)ADDY, (ULONGLONG)PUF, SIZE, FALSE };
        DeviceIoControl(hDriver, REQUEST_READWRITE, &Request, sizeof(Request), nullptr, NULL, NULL, NULL);
    }

    inline void WritePhsyical(PVOID ADDY, PVOID PUF, DWORD SIZE) {
        _ReadWrite Request = { REQUEST_SECURITY, PID, (ULONGLONG)ADDY, (ULONGLONG)PUF, SIZE, TRUE };
        DeviceIoControl(hDriver, REQUEST_READWRITE, &Request, sizeof(Request), nullptr, NULL, NULL, NULL);
    }

    inline void BSOD() {
        DeviceIoControl(hDriver, REQUEST_BSOD, 0, 0, nullptr, NULL, NULL, NULL);
    }

    inline uintptr_t GetBase() {
        uintptr_t Base = NULL;
        _Base Request = { REQUEST_SECURITY, PID, (ULONGLONG*)&Base };
        DeviceIoControl(hDriver, REQUEST_BASE, &Request, sizeof(Request), nullptr, NULL, NULL, NULL);
        return Base;
    }

    inline uintptr_t Fetch_CR3() {
        uintptr_t CR = NULL;
        _CR3 Request = { REQUEST_SECURITY, PID, (ULONGLONG*)&CR };
        DeviceIoControl(hDriver, REQUEST_CR3, &Request, sizeof(Request), nullptr, NULL, NULL, NULL);
        return CR;
    }

    inline INT32 GetPID(LPCTSTR process_name) {
        PROCESSENTRY32 pt;
        HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pt.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hsnap, &pt)) {
            do {
                if (!lstrcmpi(pt.szExeFile, process_name)) {
                    CloseHandle(hsnap);
                    PID = pt.th32ProcessID;
                    return pt.th32ProcessID;
                }
            } while (Process32Next(hsnap, &pt));
        }
        CloseHandle(hsnap);

        return { NULL };
    }
}

inline bool is_valid(const uint64_t address)
{
    if (address == 0 || address == 0xCCCCCCCCCCCCCCCC || address == 0xFFFFFFFFFFFFFFFF)
        return false;

    if (address <= 0x400000 || address > 0x7FFFFFFFFFFFFFFF)
        return false;

    return true;
}

template <typename T>
T read(uint64_t address) {
    T buffer{ };
    if (is_valid(address))
    {
        driver::ReadPhsyical((PVOID)address, &buffer, sizeof(T));
    }
    return buffer;
}

template <typename T>
T write(uint64_t address, T buffer) {
    if (is_valid(address))
    {
        driver::WritePhsyical((PVOID)address, &buffer, sizeof(T));
    }
    return buffer;
}