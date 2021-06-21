#include "proc.h"

DWORD getProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //take all processes, store in hsnap
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);
			if (Process32First(hSnap, &procEntry))
			{
				do
				{
					if (!_wcsicmp(procEntry.szExeFile, procName)) //if process id is the process name
					{
						procId = procEntry.th32ProcessID; //it sets process id
						break;
					}
				} while (Process32Next(hSnap, &procEntry)); //loop thru all processes 
			}
	}
	CloseHandle(hSnap);
	return procId;
}

uintptr_t getModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName)) //if module is same as name
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr; //put into modbaseaddr as uintptr_t
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t findDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr; 
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr; 
}	