// bdc 0x0882c208 BtlHudDraw
#include "bdc.h"

/* Draw method of the HUD/talk task (id 110, vtable slot 4): when window 0 is active
   (`UiGetWindowActive`(0)) and `phase` is 0..9, calls that phase's draw method through the
   pointer-to-member table `g_btlHudDrawTable`. A virtual entry (`index != 0`) reads the vtable at
   the offset held in `pfn` and calls entry `index`, adding its delta. */

void BtlHudDraw(BtlHud *self)
{
    const MemberFnPtr *e;
    u8 *obj;
    void *fn;

    if (UiGetWindowActive(0) == 0) {
        return;
    }
    if (self->phase < 0 || (u32)self->phase >= 10) {
        return;
    }
    e = &g_btlHudDrawTable[self->phase];
    obj = (u8 *)self + e->delta;
    fn = e->pfn;
    if (e->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)e->pfn);
        const VtblEntry *entry = &vtbl[e->index];

        obj += entry->delta;
        fn = entry->fn;
    }
    ((void (*)(void *))fn)(obj);
}
