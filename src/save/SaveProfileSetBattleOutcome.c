// bdc 0x089b1de4 SaveProfileSetBattleOutcome
#include "bdc.h"

/* Records the outcome of a rank/hologram battle in profile word 0x32 (0 win, 1 loss, 2 other, as
   passed by `BtlGetExitScene`), clears bit 1 and sets bit 8 of word 0x30
   (`SaveProfileModifyWord30Bits`), then on outcome 0 clears the hologram placement
   (`SaveProfileClearPlacedHolograms`) and on 1 or 2 adds the pending points
   (`SaveProfileApplyPendingPoints`). */

void SaveProfileSetBattleOutcome(u8 outcome)

{
  uint value;
  SaveProfile *self;
  
  value = (uint)outcome;
  self = SaveGetProfile();
  SaveProfileSetWord(self,0x32,value);
  SaveProfileModifyWord30Bits('\0',1);
  SaveProfileModifyWord30Bits('\x01',8);
  if (outcome == '\0') {
    SaveProfileClearPlacedHolograms();
  }
  else if (value < 2) {
    SaveProfileApplyPendingPoints();
  }
  else if (value < 3) {
    SaveProfileApplyPendingPoints();
  }
  return;
}

