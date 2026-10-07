// bdc 0x0884b2bc BtlMainDraw
#include "bdc.h"

/* Draw method of the main battle-scene task (id 100, vtable slot 4): calls the draw method
   selected by `drawPhase` through the `MemberFnPtr` table `g_btlMainDrawTable` (e.g.
   `BtlMainDrawScene`): `this` is adjusted by the entry's `delta`; a virtual entry (`index != 0`)
   reads the vtable at the offset held in `pfn` and calls entry `index`, adding its delta. */
void BtlMainDraw(BtlMain *self)
{
    const MemberFnPtr *e = &g_btlMainDrawTable[self->drawPhase];
    u8 *obj = (u8 *)self + e->delta;
    void *fn = e->pfn;

    if (e->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)e->pfn);
        const VtblEntry *entry = &vtbl[e->index];

        fn = entry->fn;
        obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
}
