// bdc 0x08888b10 BtlCombatIsSelectedArtReady
#include "bdc.h"

/* Returns 1 when the selected art id (`selectedArt`, -1 = none) equals the art id of one of the
   three art slots and `BtlCombatIsArtSlotReady` accepts that slot index (which it clamps to
   0..1, so slot 2 is tested as slot 1); otherwise 0. */
int BtlCombatIsSelectedArtReady(BtlCombatState *combat)
{
    int slot;

    if (combat->selectedArt != -1) {
        for (slot = 0; slot < 3; slot++) {
            if (combat->artIds[slot] == combat->selectedArt &&
                BtlCombatIsArtSlotReady(combat, slot) != 0) {
                return 1;
            }
        }
    }
    return 0;
}
