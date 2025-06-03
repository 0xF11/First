#include "everything.h"
#include "Proc.h"

int main()
{
	DWORD procID = GetProcId(L"ac_client.exe");
	// Module base Addr
	uintptr_t moduleBaseAddr = GetModuleBaseAddr(procID, (L"ac_client.exe"));

	// Get handle to proc
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);

	// Resolves base address off pointer chain 

	uintptr_t dynamicbaseaddr = moduleBaseAddr + 0x10F4F4;

	// Resolves relevent ptrs

	std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
	std::vector<unsigned int> HealthOffsets = { 0xF8 };
	uintptr_t CurrAmmo = FindDMAAddy(hProcess, dynamicbaseaddr, ammoOffsets);
	uintptr_t CurrHealth = FindDMAAddy(hProcess, dynamicbaseaddr, HealthOffsets);


	// Read

	int health = 0;
	int NewHealth = 50000;

	int ammo = 0;
	int NewAmmo = 9999;
	std::cout << "Local Player Addr = " << "0x" << std::hex << dynamicbaseaddr << std::endl;
	std::cout << "Health addr = " << "0x" << std::hex << CurrHealth << std::endl;
	std::cout << "Current Weapon Ammo Addr = " << "0x" << std::hex << CurrAmmo << std::endl;
	std::cout << "Current Ammo = " << std::dec << ammo;
	std::cout << "Current health = " << std::dec << health;
	std::cout << "New Current Ammo = " << std::dec << NewAmmo;
	std::cout << "New Health = " << std::dec << NewHealth;
	for(int i = 0; i < 9999; i++, i--)
	{
		ReadProcessMemory(hProcess, (BYTE*)CurrHealth, &health, sizeof(health), nullptr);

		ReadProcessMemory(hProcess, (BYTE*)CurrAmmo, &ammo, sizeof(ammo), nullptr);

		// Write to ammo value
		WriteProcessMemory(hProcess, (BYTE*)CurrHealth, &NewHealth, sizeof(NewHealth), nullptr);
	
		WriteProcessMemory(hProcess, (BYTE*)CurrAmmo, &NewAmmo, sizeof(NewAmmo), nullptr);

	};
