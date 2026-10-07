// bdc 0x088f4668 GameFieldCharSetDeletePlacement
#include "bdc.h"

/* Destroys the placement record `placements[slot]` (pointer table at `g_gameEventLocationBlock`)
   through its virtual destructor (flag 3) and clears the slot. */

void GameFieldCharSetDeletePlacement(void *mgr, u8 slot)
{
    GameFieldPlacement **placements = (GameFieldPlacement **)g_gameEventLocationBlock;
    GameFieldPlacement *obj = placements[slot];

    if (obj != NULL) {
        const VtblEntry *entry = &obj->vtbl[1];

        ((void (*)(void *, int))entry->fn)((u8 *)obj + entry->delta, 3);
        placements[slot] = NULL;
    }
}
