// bdc 0x08982794 UiCollectionCardUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiCollectionCard screen (task id 313): runs the current
   phase handler from the 4-entry pointer-to-member phase table `g_uiCollectionCardPhaseFns` (indexed by `phase`,
   +0x28), then `UiScreenUpdateCommon` and, unless a close was requested, `UiScreenUpdateBg`. */

void UiCollectionCardUpdate(UiCollectionCard *self)
{
    int phase = self->base.phase;
    u8 closeRequested;

    if (phase >= 0 && (unsigned)phase < 4) {
        const MemberFnPtr *member = &g_uiCollectionCardPhaseFns[phase];
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
    closeRequested = self->base.closeRequested;
    UiScreenUpdateCommon(&self->base);
    if (closeRequested == 0) {
        UiScreenUpdateBg(&self->base);
    }
}
