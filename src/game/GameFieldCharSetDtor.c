// bdc 0x088f4024 GameFieldCharSetDtor
#include "bdc.h"

/* Destructor of the character-set manager: clears `g_gameFieldCharSet` and frees it when `flags & 1`. */
void GameFieldCharSetDtor(void *mgr, u32 flags)
{
    if (mgr != NULL) {
        g_gameFieldCharSet = NULL;
        if (flags & 1) {
            MemLock();
            MemFree(mgr, NULL, 0);
            MemUnlock();
        }
    }
}
