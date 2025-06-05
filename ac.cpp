#include "everything.h"
#include "Proc.h"




int main()
{
    DWORD ProcID = GetProcId(L"ac_client.exe");
    uintptr_t mbaseaddr = GetModuleBaseAddr(ProcID, L"ac_client.exe");

    HANDLE hP = 0;
    hP = OpenProcess(PROCESS_ALL_ACCESS, NULL, ProcID);

    uintptr_t DMA = mbaseaddr + 0x10f4f4;
    std::cout << "0x" << std::hex << DMA;

    std::vector<unsigned int>HealthOffset{ 0xf8 };
    std::vector<unsigned int>AmmoOff{ 0x374, 0x14, 0x0 };
    std::vector<unsigned int>SniperCoolOff{ 0x174 };
    uintptr_t healthaddr = FindDMAAddy(hP, DMA, HealthOffset);
    uintptr_t ammoaddr = FindDMAAddy(hP, DMA, AmmoOff);
    uintptr_t Cooldownaddr = FindDMAAddy(hP, DMA, SniperCoolOff);
    std::cout << "Health addr = 0x" << std::hex << healthaddr;
    std::cout << "\n\n";

    int oHP = 0;
    ReadProcessMemory(hP, (BYTE*)healthaddr, &oHP, sizeof(oHP), 0);
    std::cout << std::dec << oHP;


    int i = 0;
    
    do
    {

        int nHP = 9999999;
        WriteProcessMemory(hP, (BYTE*)healthaddr, &nHP, sizeof(nHP), 0);
        uintptr_t ammoaddr = FindDMAAddy(hP, DMA, AmmoOff);
        int oAmmo = 0;
        ReadProcessMemory(hP, (byte*)ammoaddr, &oAmmo, sizeof(oAmmo), 0);
        int NAmmo = 777;
        WriteProcessMemory(hP, (byte*)ammoaddr, &NAmmo, sizeof(NAmmo), 0);
        int NoCool = 0;
        WriteProcessMemory(hP, (byte*)Cooldownaddr, &NoCool, sizeof(NoCool), 0);
    } while (i < 5);


    // Loop to run shit infinetly


}

