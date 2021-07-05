#ifndef LMGOOC_H
#define LMGOOC_H

#include <sys/mman.h>
#include <dlfcn.h>
#include <link.h>

#ifdef _DEBUG
#include "log.h"
#endif

class LMGOOC
{
public:
    struct CVector { float x, y, z; };

    LMGOOC();
    
    static bool CVehicle__CanPedStepOutCar(uintptr_t vehicle, int a2);
    static bool CVehicle__CanPedJumpOutCar(uintptr_t vehicle, void* ped);
    
protected:
    uintptr_t findMethod(const char* name);
};

#endif // LMGOOC_H
