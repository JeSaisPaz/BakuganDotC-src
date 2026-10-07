// bdc 0x08887d3c BtlCombatDrainEnergy
#include "bdc.h"

/* Reduces the energy gauge by `amount` (through `BtlCombatSetEnergy`, so it clamps at 0). For a
   player unit (`isPlayer != 0`) whose drain would take the gauge to <= 0 while the gauge is not yet
   <= 0, it first plays the "out of energy" sound 0x20012f through the sound manager, unless that
   sound is already playing (`SndManagerIsSoundWordPlaying`) or there is no manager
   (`SndHasManager`, `SndManagerPlay`). The energy is read again for each test. */

void BtlCombatDrainEnergy(float amount, BtlCombatState *combat)
{
    BtlBakugan *owner = combat->owner;

    if (owner->isPlayer != 0) {
        if (BtlCombatGetEnergy(combat) - amount <= 0.0f && !(BtlCombatGetEnergy(combat) <= 0.0f)) {
            if (SndManagerIsSoundWordPlaying(SndGetManager(), 0x20012f) == 0 && SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x20012f, 0, 0);
            }
        }
    }
    BtlCombatSetEnergy(BtlCombatGetEnergy(combat) - amount, combat);
}
