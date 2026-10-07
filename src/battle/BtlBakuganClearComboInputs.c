// bdc 0x08863350 BtlBakuganClearComboInputs
#include "bdc.h"

/* Empties a unit's buffered melee-combo inputs: zeroes the 8-entry input buffer `+0x5d8..+0x5df`
   (one byte per combo step: 1 = attack, 2 = attack with command flag 0x10000), its fill count
   `+0x5d4` and the count of queued, not yet started combo steps `+0x5c4`. Called by
   `BtlBakuganSetControlLock` when a non-player unit is locked, so no buffered combo continues
   after the lock. */

void BtlBakuganClearComboInputs(BtlBakugan *unit)

{
  int i;

  for (i = 0; i < 8; i++) {
    unit->comboInputs[i] = 0;
  }
  unit->comboInputCount = 0;
  unit->queuedComboSteps = 0;
}
