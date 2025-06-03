#include "everything.h"
#include "Proc.h"

DWORD GetProcId(const wchar_t* ProcName)
{
	DWORD procID = 0; // Error checking
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	// Getting a snapshot of the process we are fucking with, Set it to hSnap 

	if (hSnap != INVALID_HANDLE_VALUE) // All this error checking or mitigation does is only runs our function if we get a snapshot.
	{
	
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry)) // Grabs our first process in a our 32Snapshot, & stores it to proc entry
		{
			

			// This compares all the file names in the windows snapshot against the name of the file we will be literally setting ((  L("proc")  ))
			// When it finds our process name and our comparision returns true
			// it will resolve the process id and set it to procID
			// tHEN WE WILL BREAK OUT OF THE LOOP
			
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, ProcName)) 
				{
					procID = procEntry.th32ProcessID;  // 
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
			// All im getting from the while statement here is
			// We will constantly be looping through until we resolve our processID
		}
		CloseHandle(hSnap); // Closing our handle to prevent memory leaks (like assigning a pointer bytes in memory, we close it)
		return procID; // Gives us our processID for our given processname
	}
}


uintptr_t GetModuleBaseAddr(DWORD ProcID, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0; // error check
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcID); // Creating a snapshot of the module and 
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(hSnap, &modEntry))
		{

			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, & modEntry));
			
		}
		CloseHandle(hSnap);
		return modBaseAddr;

	}
}

// hProc - Our handle to a process, aka Reading another processes memory
// ptr - "ac_client"+10F4F4 - our base pointer
// std::vector<unsigned int> offsets - a vector pretty much lets us have our own controlled array
// Which dynamically allocates however many "offsets" we will need
// For example, in assault cube your current weapon offset is 0x374, 0x14, 0
// Instead of making FindDMAAddy1OF and so on and so fourt, our vector will let us make our on user controlled offset arrays

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr; // Assigning our base address to a variable

	for (int i = 0; i < offsets.size(); i++) // I will loop through our offsets until completed
	{
		// This function does this
		// Gets our handle to the process
		// We cast our LPCVoid (This is a string pointer AKA L("ac_client.exe")
		// Referencing addr (+10F4F4)
		// Leaving the last space null because it is not really needed

		//	hProc,   The handle to the target process (the one we're reading)
		//  -
		//	(BYTE*)addr, The address inside that process where we want to read from
		//	& addr,      Where we want to store what we read (a pointer to our variable)
		//	sizeof(addr),  How many bytes we want to read (same size as uintptr_t — 4 or 8 bytes)
		//	0              We’re ignoring how many bytes were actually read
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);

		// This is dereferencing our multisystem pointer
		addr += offsets[i];
	}
	return addr;

}
