// bdc 0x08987d7c UiCollectionTheaterUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiCollectionTheater screen (task id 315): runs the current
   phase handler from the 5-entry pointer-to-member phase table `g_uiCollectionTheaterPhaseFns` (indexed by `phase`,
   +0x28), then `UiScreenUpdateCommon` and, unless a close was requested, `UiScreenUpdateBg`. */

void UiCollectionTheaterUpdate(UiScreen *screen)
{
    int phase = screen->phase;
    u8 closeRequested;

    if (phase >= 0 && (unsigned)phase < 5) {
        const MemberFnPtr *member = &g_uiCollectionTheaterPhaseFns[phase];
        u8 *obj = (u8 *)screen + member->delta;
        void *fn = member->pfn;

        if (member->index != 0) {
            const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)member->pfn);
            const VtblEntry *entry = &vtbl[member->index];

            fn = entry->fn;
            obj += entry->delta;
        }
        ((void (*)(void *))fn)(obj);
    }
    closeRequested = screen->closeRequested;
    UiScreenUpdateCommon(screen);
    if (closeRequested == 0) {
        UiScreenUpdateBg(screen);
    }
}
