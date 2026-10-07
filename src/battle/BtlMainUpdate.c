// bdc 0x0884b244 BtlMainUpdate
#include "bdc.h"

/* Update method of the main battle-scene task (id 100, vtable slot 2): increments the battle
   frame counter `g_btlFrameCount`, then calls the phase method selected by `phase` through the
   `MemberFnPtr` table `g_btlMainUpdateTable` (e.g. `BtlMainPhaseBattle`,
   `BtlMainPhaseExit`, `BtlMainPhaseIntro`): `this` is adjusted by the entry's `delta`; a
   virtual entry (`index != 0`) reads the vtable at the offset held in `pfn` and calls entry
   `index`, adding its delta. */
void BtlMainUpdate(BtlMain *self)
{
    const MemberFnPtr *e;
    u8 *obj;
    void *fn;

    g_btlFrameCount++;
    e = &g_btlMainUpdateTable[self->phase];
    obj = (u8 *)self + e->delta;
    fn = e->pfn;
    if (e->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)e->pfn);
        const VtblEntry *entry = &vtbl[e->index];

        fn = entry->fn;
        obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
}
