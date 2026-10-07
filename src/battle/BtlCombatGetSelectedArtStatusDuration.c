// bdc 0x088890c4 BtlCombatGetSelectedArtStatusDuration
#include "bdc.h"

/* Returns the status duration in frames (`BtlArtRecord``.statusFrames`, sign-extended) of the
   selected art's record when it is a status art (`BtlCombatSelectedArtAppliesStatus`) and has a
   record, else 0. */
s32 BtlCombatGetSelectedArtStatusDuration(BtlCombatState *combat)
{
    const BtlArtRecord *record = (const BtlArtRecord *)BtlCombatGetSelectedArtRecord(combat);
    s32 frames = 0;

    if (BtlCombatSelectedArtAppliesStatus(combat) != 0 && record != NULL) {
        frames = record->statusFrames;
    }
    return frames;
}
