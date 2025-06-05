#include "everything.h"
#include "Proc.h"


// proof of conc that i learned how to write it on my own
int main() {
	DWORD procid = GetProcId(L"ac_client.exe");


	uintptr_t modbase = GetModuleBaseAddr(procid, L"ac_client.exe");


	HANDLE hproc = 0;
	hproc = OpenProcess(PROCESS_ALL_ACCESS, 0, procid);


	uintptr_t dma = modbase + 0x10f4f4;

	std::vector<unsigned int> hpoff{ 0xf8 };
	uintptr_t hpaddr = FindDMAAddy(hproc, dma, hpoff);
	std::vector<unsigned int> ammooff{ 0x374,0x14,0x0 };
	uintptr_t ammoaddr = FindDMAAddy(hproc, dma, ammooff);


	while (true) {
		int hpmax = 1337;
		uintptr_t ammoaddr = FindDMAAddy(hproc, dma, ammooff);
		int newammo = 99999;

		WriteProcessMemory(hproc, (byte*)hpaddr, &hpmax, sizeof(hpmax), 0);
		WriteProcessMemory(hproc, (byte*)ammoaddr, &newammo, sizeof(newammo), 0);
	}



}
