// bdc 0x088be250 GameFieldUpdate
#include "bdc.h"

/* Update method (vtable slot 2) of the field (world map) scene task, task id 500
   (`GameFieldCtor`, 0x7a0 bytes, vtable `0x08af2cfc`): calls the phase handler
   `g_gameFieldUpdateFns[phase]` (`MemberFnPtr` table, phase at `+0x618`): 0 `GameFieldPhaseLoad`, 1
   `GameFieldPhaseMain`, 2..7 `GameFieldPhasePause`, `GameFieldPhaseExit`, `GameFieldPhaseRepair`, `GameFieldPhaseSettings`,
   `GameFieldPhaseScreen380`, `GameFieldPhaseSave`. */

void GameFieldUpdate(CoreTask *task)
{
    const MemberFnPtr *member = &g_gameFieldUpdateFns[((GameFieldTask *)task)->phase];
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
