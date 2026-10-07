// bdc 0x08a29f08 BtlBakuganRunState
#include "bdc.h"

/* Runs the per-frame handler of the Bakugan's current battle state: takes the `MemberFnPtr`
   entry `g_btlBakuganStateTable``[state]`, adjusts `this` by its `delta` and, for a virtual
   entry (`index != 0`), reads the vtable at the offset held in `pfn` and calls entry `index`
   with that entry's delta added (vtable index `26 + state`, i.e. `BtlBakuganState00Update` ..
   `BtlBakuganState21Update` for the base class); a non-virtual entry calls `pfn` directly.
   Installed in vtable slot `+0x180` of the battle unit classes and called once per frame by the
   unit update. */
void BtlBakuganRunState(BtlBakugan *self)
{
    const MemberFnPtr *e = &g_btlBakuganStateTable[self->state];
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
