// bdc 0x0882c138 BtlHudUpdate
#include "bdc.h"

/* Update of the HUD/talk task (id 110, vtable slot 2): in netplay (`SaveGetProfileFlag0`) with
   the battle task alive, first runs the battle quit prompt on it (`BtlMainUpdateQuitPrompt`);
   then, for `phase` 0..9, calls the `MemberFnPtr` `g_btlHudPhaseFns[phase]`
   (`g_btlHudPhaseFns`) on the HUD. */
void BtlHudUpdate(BtlHud *self)
{
    if (SaveGetProfileFlag0() != 0 && BtlCameraTaskExists() != 0) {
        BtlMainUpdateQuitPrompt(BtlGetCameraTask());
    }
    if (self->phase >= 0 && self->phase < 10) {
        const MemberFnPtr *member = &g_btlHudPhaseFns[self->phase];
        u8 *obj = (u8 *)self + member->delta;
        void *fn = member->pfn;

        /* GCC 2.x pointer-to-member: index != 0 is a virtual slot, pfn then holds the vptr offset */
        if (member->index != 0) {
            const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)member->pfn);
            const VtblEntry *entry = &vtbl[member->index];

            fn = entry->fn;
            obj += entry->delta;
        }
        ((void (*)(void *))fn)(obj);
    }
}
