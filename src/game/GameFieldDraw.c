// bdc 0x088be2b8 GameFieldDraw
#include "bdc.h"

/* Draw method (vtable slot 4) of the field (world map) scene task, task id 500 (`GameFieldCtor`,
   0x7a0 bytes, vtable `0x08af2cfc`): calls the phase's draw handler from the `MemberFnPtr` table
   `g_gameFieldDrawFns[phase]` (phase at `+0x618`): phase 0 `GameFieldDrawLoading`, phases 1..4, 6, 7
   `GameFieldDrawScene`, phase 5 `0x088c35b8`. */

void GameFieldDraw(CoreTask *task)
{
    const MemberFnPtr *member = &g_gameFieldDrawFns[((GameFieldTask *)task)->phase];
    u8 *self = (u8 *)task + member->delta;
    void *fn = member->pfn;

    if (member->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(self + (intptr_t)member->pfn);
        const VtblEntry *entry = &vtbl[member->index];

        fn = entry->fn;
        self += entry->delta;
    }
    ((void (*)(void *))fn)(self);
}
