// bdc 0x089ababc UiPauseSettingsUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiPauseSettings screen (task id 301): runs the current
   phase handler from the 4-entry pointer-to-member phase table `g_uiPauseSettingsPhaseFns` (indexed by `phase`,
   +0x28), then `UiScreenUpdateCommon` and, unless a close was requested, `UiScreenUpdateBg`. */

void UiPauseSettingsUpdate(UiPauseSettings *self)
{
    int phase = self->base.phase;
    u8 closeRequested;

    if (phase >= 0 && (unsigned)phase < 4) {
        const MemberFnPtr *member = &g_uiPauseSettingsPhaseFns[phase];
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
