// bdc 0x088e9f44 GameFieldGuardBlindDtor
#include "bdc.h"

/* Destructor of the guard-blind timer: frees it when `flags & 1`. */
void GameFieldGuardBlindDtor(void *blind, u32 flags)
{
    if (blind != NULL && (flags & 1) != 0) {
        MemLock();
        MemFree(blind, NULL, 0);
        MemUnlock();
    }
}
