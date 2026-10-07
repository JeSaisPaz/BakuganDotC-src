// bdc 0x089578dc UiEquipUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiEquip screen (task id 302): runs the current phase
   handler from the 7-entry pointer-to-member phase table `0x08a9d5f8` (indexed by `phase`, +0x28),
   then `UiEquipUpdateModels`, then `UiScreenUpdateCommon` and, unless a close was requested,
   `UiScreenUpdateBg`. */

void UiEquipUpdate(UiEquip *self)
{
    int phase = self->base.phase;
    u8 closeRequested;

    if (phase >= 0 && (unsigned)phase < 7) {
        const MemberFnPtr *member = &g_uiEquipPhaseFns[phase];
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
    UiEquipUpdateModels(self);
    closeRequested = self->base.closeRequested;
    UiScreenUpdateCommon(&self->base);
    if (closeRequested == 0) {
        UiScreenUpdateBg(&self->base);
    }
}
