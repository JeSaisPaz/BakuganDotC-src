// bdc 0x08888c40 BtlCombatChargeArtInPinch
#include "bdc.h"

/* Pinch charging of special-art slot `slot` of a living `BtlCombatState`: when
   `BtlCombatArtSlotCanCharge``(combat, slot, 1)` holds (which also ticks the slot's cooldown)
   and `hp / maxHp` is below 0.2 (max HP from `BtlCombatGetMaxHp`, converted as unsigned), adds
   `arts[tier].pinchCharge * arts[artId % 4].chargeRate` to the slot's charge (art id of the slot,
   C remainder so a negative id gives a non-positive row) and caps it at 10000. The stat rows are
   read as whole-entry copies, as the compiler does. */
void BtlCombatChargeArtInPinch(BtlCombatState *combat, s32 slot, s32 tier)
{
    BtlArtStatEntry tierRow = combat->stats->arts[tier];
    BtlArtStatEntry artRow;
    float charge;

    if (combat->dead != 0) {
        return;
    }
    if (BtlCombatArtSlotCanCharge(combat, slot, 1) == 0) {
        return;
    }
    if (!(combat->hp / (float)(u32)BtlCombatGetMaxHp(combat) < 0.200000003f)) {
        return;
    }
    artRow = combat->stats->arts[combat->artIds[slot] % 4];
    charge = combat->artCharge[slot] + tierRow.pinchCharge * artRow.chargeRate;
    combat->artCharge[slot] = charge;
    if (!(charge <= 10000.0f)) {
        combat->artCharge[slot] = 10000.0f;
    }
}
