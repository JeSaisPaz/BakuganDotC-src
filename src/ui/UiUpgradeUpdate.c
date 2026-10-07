// bdc 0x08912b18 UiUpgradeUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the Bakugan upgrade screen (task id 490): runs the phase
   handler from the 4-entry table `0x08a9bc08` and `UiScreenUpdateCommon`. */

void UiUpgradeUpdate(UiUpgrade *self)
{
    int phase = self->base.phase;

    if (phase >= 0 && (unsigned)phase < 4) {
        const MemberFnPtr *member = &g_uiUpgradePhaseFns[phase];
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
    if (self->model != NULL) {
        GfxModel *model = (GfxModel *)self->model;
        const VtblEntry *entry = (const VtblEntry *)model->base.vtable + 7;

        ((void (*)(void *))entry->fn)((u8 *)model + entry->delta);
    }
    UiScreenUpdateCommon(&self->base);
}
