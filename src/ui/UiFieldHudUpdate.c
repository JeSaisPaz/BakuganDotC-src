// bdc 0x088cee58 UiFieldHudUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the field HUD (task id 3001). While
   g_uiFieldHudResetRequest is set it only acts once phase >= 2: UiFieldHudResetLayout, clears the
   request, sets g_uiFieldHudEnabled and restarts at phase 2 / step 0. Otherwise: if the field scene
   task (id 500) is gone, phase becomes 5; a phase >= 5 (unsigned) removes and destroys the HUD task
   but still falls through. Then, unless UiFieldHudIsSuspended or task 410 exists, runs the
   g_uiFieldHudPhaseFns handler of phases 0..4 (GCC 2.x member-function pointer). */

void UiFieldHudUpdate(UiFieldHud *self)
{
    CoreTask *task;
    bool fieldAlive;
    s32 phase;

    if (g_uiFieldHudResetRequest != 0) {
        if (self->base.phase > 1) {
            UiFieldHudResetLayout(self);
            g_uiFieldHudResetRequest = 0;
            g_uiFieldHudEnabled = 1;
            self->base.phase = 2;
            self->base.phaseStep = 0;
        }
        return;
    }

    task = (CoreTask *)CoreTaskFind(500);
    fieldAlive = false;
    if (task != NULL && CoreTaskIsAlive(task) != 0) {
        fieldAlive = true;
    }
    if (!fieldAlive) {
        self->base.phase = 5;
    }
    if ((u32)self->base.phase >= 5) {
        CoreTaskRemove(&self->base.base, true);
    }
    if (UiFieldHudIsSuspended(self)) {
        return;
    }
    if (CoreTaskExists(0x19a) != 0) {
        return;
    }
    phase = self->base.phase;
    if (phase >= 0 && phase < 5) {
        const MemberFnPtr *member = &g_uiFieldHudPhaseFns[phase];
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
}
