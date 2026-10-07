// bdc 0x088f48dc GameFieldCharSetFreezeAll
#include "bdc.h"

/* Calls virtual slot 16 (freeze) on every placed actor. */

void GameFieldCharSetFreezeAll(GameFieldCharSet *mgr)
{
    u8 i = 0;

    do {
        GameFieldPlacement *obj = mgr->actors[i];
        const VtblEntry *entry = &obj->vtbl[16];

        ((void (*)(void *))entry->fn)((u8 *)obj + entry->delta);
        i++;
    } while (i < mgr->placedCount);
}
