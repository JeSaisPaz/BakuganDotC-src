// bdc 0x088b7744 BtlStageGetItemOdds
#include "bdc.h"

/* Returns the 12-byte item-odds row `row` (0..4) of the current stage (script global 1, clamped
   0..0x26) from `g_btlStageItemOdds` (`BtlItemOdds` rows read by
   `BtlItemRollKind`). */

int *BtlStageGetItemOdds(int row)

{
  int stage = g_scriptGlobalVars[1];

  if (stage < 0) {
    stage = 0;
  }
  else if (stage > 0x26) {
    stage = 0x26;
  }
  return &g_btlStageItemOdds[stage][row].pctA;
}
