// bdc 0x08888ec4 BtlCombatChargeArtsFromCombo
#include "bdc.h"

/* Adds combo charge to both special-art gauges of a living unit (nothing while `dead`): for each
   slot 0..1 that `BtlCombatArtSlotCanCharge``(combat, slot, 0)` accepts and whose art id is
   not negative, `artCharge[slot]` grows by `(combo * 100 + 1000) * stats->arts[id % 4].chargeRate`
   and is set to 10000 (a full gauge) unless the sum is <= 10000 (so NaN also becomes 10000).
   The binary also loads the whole 12-byte art row into a dead stack copy. */
void BtlCombatChargeArtsFromCombo(BtlCombatState *combat, int combo)
{
    float gain;
    int slot;

    if (combat->dead) {
        return;
    }
    gain = (float)combo * 100.0f + 1000.0f;
    for (slot = 0; slot < 2; slot++) {
        s32 artId;
        float charge;

        if (!BtlCombatArtSlotCanCharge(combat, slot, 0)) {
            continue;
        }
        artId = combat->artIds[slot];
        if (artId < 0) {
            continue;
        }
        charge = combat->artCharge[slot] + gain * combat->stats->arts[artId % 4].chargeRate;
        combat->artCharge[slot] = charge;
        if (!(charge <= 10000.0f)) {
            combat->artCharge[slot] = 10000.0f;
        }
    }
}
