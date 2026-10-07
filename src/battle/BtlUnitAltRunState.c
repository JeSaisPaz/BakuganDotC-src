// bdc 0x0885bd74 BtlUnitAltRunState
#include "bdc.h"

/* State-dispatch override of the battle-unit subclass with vtable `0x08af1c94` (the 0x6e0-byte
   unit `BtlCreateBakugan` builds for non-playable kinds): same scheme as `BtlBakuganRunState`
   but through the `MemberFnPtr` table `g_btlUnitAltStateTable` indexed by `state`. `this` is
   adjusted by the entry's `delta`; a virtual entry (`index != 0`) reads the vtable at the offset
   held in `pfn` and calls entry `index`, adding its delta. */
void BtlUnitAltRunState(BtlBakugan *unit)
{
    const MemberFnPtr *e = &g_btlUnitAltStateTable[unit->state];
    u8 *obj = (u8 *)unit + e->delta;
    void *fn = e->pfn;

    if (e->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)e->pfn);
        const VtblEntry *entry = &vtbl[e->index];

        fn = entry->fn;
        obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
}
