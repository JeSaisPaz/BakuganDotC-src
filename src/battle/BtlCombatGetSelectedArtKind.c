// bdc 0x08888ff0 BtlCombatGetSelectedArtKind
#include "bdc.h"

/* Returns `BtlArtGetKind` of the selected special art `+0xe0` of a `BtlCombatState`, or 0 when
   none is selected (-1). `BtlBakuganState17Update` uses kind 2 to go to state 0xb and kinds 3..4
   to clear the timed statuses. */

s32 BtlCombatGetSelectedArtKind(BtlCombatState *combat)
{
    if (combat->selectedArt != -1) {
        return BtlArtGetKind(combat, combat->selectedArt);
    }
    return 0;
}
