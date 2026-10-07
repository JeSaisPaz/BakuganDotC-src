// bdc 0x08992600 UiUnlockCodeUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiUnlockCode screen (task id 316): runs the current phase
   handler from the 6-entry pointer-to-member phase table `g_uiUnlockCodePhaseFns` (indexed by `phase`, +0x28),
   then removes and deletes itself (`CoreTaskRemove``(this, true)`) when its own done byte `+0x74`
   is set. Unlike most screens it does not call `UiScreenUpdateCommon`. */

void UiUnlockCodeUpdate(UiUnlockCode *self)
{
    int phase = self->base.phase;

    if (phase >= 0 && (unsigned)phase < 6) {
        const MemberFnPtr *member = &g_uiUnlockCodePhaseFns[phase];
        u8 *obj = (u8 *)self + member->delta;
        void *fn = member->pfn;

        if (member->index != 0) {
            const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)member->pfn);
            const VtblEntry *entry = &vtbl[member->index];

            fn = entry->fn;
            obj += entry->delta;
        }
        ((void (*)(void *))fn)(obj);
    }
    if (self->removeRequested != 0) {
        CoreTaskRemove((CoreTask *)self, true);
    }
}
