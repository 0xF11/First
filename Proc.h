#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <Tlhelp32.h>



DWORD GetProcId(const wchar_t* ProcName);

uintptr_t GetModuleBaseAddr(DWORD ProcID, const wchar_t* modName);

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int>); 


