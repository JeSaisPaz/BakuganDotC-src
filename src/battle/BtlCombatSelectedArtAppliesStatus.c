// bdc 0x0888902c BtlCombatSelectedArtAppliesStatus
#include "bdc.h"

/* Returns 1 when the selected special art (`BtlCombatGetSelectedArtKind`) is of kind 3 or 4, the
   kinds whose record carries a status id and duration. */
s32 BtlCombatSelectedArtAppliesStatus(BtlCombatState *combat)
{
    s32 kind = BtlCombatGetSelectedArtKind(combat);
    return kind >= 3 && kind <= 4;
}
