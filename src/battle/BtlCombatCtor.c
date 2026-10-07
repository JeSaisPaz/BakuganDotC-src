// bdc 0x088866cc BtlCombatCtor
#include "bdc.h"

/* Constructor of a `BtlCombatState`: stores the vtable `g_btlCombatStateVtbl` and runs
   `BtlCombatInit`. Unit constructors call it on the embedded state (`unit + 0x434`). */
void BtlCombatCtor(BtlCombatState *combat)
{
    combat->vtable = g_btlCombatStateVtbl;
    BtlCombatInit(combat);
}
