// bdc 0x08889108 BtlCombatActivateSelectedArt
#include "bdc.h"

/* Fires the selected special art of a `BtlCombatState` (nothing when `selectedArt` is -1): finds
   the first slot 0..2 whose `artIds[slot]` equals `selectedArt` and whose charge is not below
   10000 (a full gauge). For a status art (`BtlCombatSelectedArtAppliesStatus`) it stores the
   art's status id in `listedStatusIds[slot]` and applies it (`BtlCombatApplyStatus`) for the
   record's duration × (1, + 0.5 with upgrade 0x13, + 1.0 with upgrade 0x14), truncated to int.
   Then `artCooldown[slot]` gets `stats->arts[selectedArt % 4].cooldown`, the slot's charge is
   emptied and `artReadyFrames` is cleared. */
void BtlCombatActivateSelectedArt(BtlCombatState *combat)
{
    s32 art = combat->selectedArt;
    int slot;

    if (art == -1) {
        return;
    }
    for (slot = 0; slot < 3; slot++) {
        float cooldown;
        float scale;

        if (combat->artIds[slot] != art || combat->artCharge[slot] < 10000.0f) {
            continue;
        }
        cooldown = combat->stats->arts[art % 4].cooldown;
        scale = 1.0f;
        if (BtlCombatHasUpgrade(combat, 0x13)) {
            scale = 1.0f + 0.5f;
        }
        if (BtlCombatHasUpgrade(combat, 0x14)) {
            scale = scale + 1.0f;
        }
        if (BtlCombatSelectedArtAppliesStatus(combat)) {
            s32 status;
            s32 duration;

            combat->listedStatusIds[slot] = BtlCombatGetSelectedArtStatus(combat);
            status = BtlCombatGetSelectedArtStatus(combat);
            duration = BtlCombatGetSelectedArtStatusDuration(combat);
            BtlCombatApplyStatus(combat, status, (s32)((float)duration * scale));
        }
        combat->artCooldown[slot] = cooldown;
        combat->artCharge[slot] = 0.0f;
        combat->artReadyFrames = 0;
        return;
    }
}
