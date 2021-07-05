#include "LMGOOC.h"

LMGOOC g_instance;

LMGOOC::LMGOOC()
{
    auto pAddr = findMethod("_ZNK8CVehicle16CanPedJumpOutCarEP4CPed");
	if (!pAddr) return;
    mprotect((void*)(pAddr & 0xFFFFF000), PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
    *reinterpret_cast<uintptr_t*>(pAddr) = reinterpret_cast<uintptr_t>(LMGOOC::CVehicle__CanPedJumpOutCar);

    pAddr = findMethod("_ZNK8CVehicle16CanPedStepOutCarEb");
	if (!pAddr) return;
    mprotect((void*)(pAddr & 0xFFFFF000), PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
    *reinterpret_cast<uintptr_t*>(pAddr) = reinterpret_cast<uintptr_t>(&LMGOOC::CVehicle__CanPedStepOutCar);
}

bool LMGOOC::CVehicle__CanPedStepOutCar(uintptr_t vehicle, int a2)
{
    auto& moveSpeed = *reinterpret_cast<CVector*>(vehicle + 0x48);
    return (moveSpeed.x * moveSpeed.x +
            moveSpeed.y * moveSpeed.y +
            moveSpeed.z * moveSpeed.z
            ) < 0.1f;
}

bool LMGOOC::CVehicle__CanPedJumpOutCar(uintptr_t vehicle, void* ped)
{
    auto& moveSpeed = *reinterpret_cast<CVector*>(vehicle + 0x48);
    return (moveSpeed.x * moveSpeed.x +
            moveSpeed.y * moveSpeed.y +
            moveSpeed.z * moveSpeed.z
            ) > 0.1f;
}

uintptr_t LMGOOC::findMethod(const char* name)
{
    auto func_addr = dlsym(dlopen("libGTASA.so", RTLD_NOW), name);
    if (!func_addr) return 0;
    
    Dl_info info;
    dladdr(func_addr, &info);
    auto lib = (uint8_t*)info.dli_fbase;
    
    auto hdr = (Elf32_Ehdr*)lib;
	auto pht = (Elf32_Phdr*)(&lib[hdr->e_phoff]);

	Elf32_Dyn* dyn = nullptr;
	for (int i = 0; i < hdr->e_phnum; ++i, pht++) {
		if (pht->p_type != 2) continue;
		dyn = (Elf32_Dyn*)(&lib[pht->p_offset]);
		break;
	}
	if (!dyn) return 0;

	uintptr_t strtab = 0;
	uintptr_t fnctab = 0;
	uintptr_t reltab = 0;
	uintptr_t jmpreltab = 0;
	for (; dyn->d_tag != 0; dyn++) {
		if (dyn->d_tag == 5) strtab = dyn->d_un.d_val;
		else if (dyn->d_tag == 6) fnctab = dyn->d_un.d_val;
		else if (dyn->d_tag == 17) reltab = dyn->d_un.d_val;
		else if (dyn->d_tag == 23) jmpreltab = dyn->d_un.d_val;
	}
	if (!strtab || !fnctab || !reltab || !jmpreltab) return 0;

	uintptr_t index = 0;
	uintptr_t address = 0;
	auto sym = (Elf32_Sym*)(&lib[fnctab]);
	for (auto i = 0; fnctab + i * sizeof(Elf32_Sym) != strtab; ++i, sym++) {
		auto name_ptr = uintptr_t(lib) + sym->st_name + strtab;
		if (strcmp((char*)name_ptr, name)) continue;
		index = i;
        address = uintptr_t(lib) + sym->st_value;
		break;
	}
	
	struct Elf32_Plt{
        uint32_t d_value;
    };

	auto rel = (Elf32_Rel*)(&lib[reltab]);
	for (auto i = 0; i < (strtab - fnctab) / sizeof(Elf32_Sym); ++i, rel++) {
		if ((rel->r_info >> 8) != index) continue;
		return uintptr_t(lib) + rel->r_offset;
	}

	auto jmprel = (Elf32_Rel*)(&lib[jmpreltab]);
	for (auto i = 0; i < (strtab - fnctab) / sizeof(Elf32_Sym); ++i, jmprel++) {
		if ((jmprel->r_info >> 8) != index) continue;
		return uintptr_t(lib) + jmprel->r_offset;
	}
	
	return 0;
}
