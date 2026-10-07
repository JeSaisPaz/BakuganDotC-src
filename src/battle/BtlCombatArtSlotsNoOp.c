// bdc 0x08889288 BtlCombatArtSlotsNoOp
#include "bdc.h"

/* Empty loop over the three special-art slots (counter 1..3); the body was compiled out, so the
   function does nothing. */
void BtlCombatArtSlotsNoOp(void)
{
    int slot;

    for (slot = 1; slot <= 3; slot++) {
    }
}
