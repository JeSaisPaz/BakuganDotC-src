// bdc 0x089c3188 SndBgmPlayerDestroyObj
#include "bdc.h"

/* Destructor of `SndBgmPlayer` (GCC 2.x `__in_chrg` style): destroys the player's mutex with
   `CoreLockDestroy``(lock, 3)` and clears the pointer; when `flags & 1` the object itself is
   freed with `MemFree` under `MemLock`/`MemUnlock`. Called by `SndBgmPlayerDestroy`. */

void SndBgmPlayerDestroyObj(SndBgmPlayer *player, u32 flags)
{
    if (player != NULL) {
        if (player->lock != NULL) {
            CoreLockDestroy(player->lock, 3);
            player->lock = NULL;
        }
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(player, NULL, 0);
            MemUnlock();
        }
    }
}
