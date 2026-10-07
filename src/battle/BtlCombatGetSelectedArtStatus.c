// bdc 0x08889098 BtlCombatGetSelectedArtStatus
#include "bdc.h"

/* Returns the `statusId` of the selected art's `BtlArtRecord`
   (`BtlCombatGetSelectedArtRecord`), 0 when none is selected. */

s32 BtlCombatGetSelectedArtStatus(BtlCombatState *combat)

{
  const BtlArtRecord *record = BtlCombatGetSelectedArtRecord(combat);

  if (record == NULL) {
    return 0;
  }
  return record->statusId;
}
