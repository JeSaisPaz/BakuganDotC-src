// bdc 0x08838328 BtlHudAdviceUpdateStageHints
#include "bdc.h"

/* Runs the stage-specific advice slots for the current stage number (script global variable
   entry 1 of `g_scriptGlobalVars`): stage 4 → `BtlHudAdviceTutorialIntro` (slot 0x16),
   stage 5 → `BtlHudAdviceStage5Hint` (slot 0x17), stage 0x24 → `BtlHudAdviceStage36HintA`,
   `BtlHudAdviceStage36HintB` and `BtlHudAdviceStage36HintC` (slots 0x18, 0x19, 0x1a) in
   that order; nothing on other stages. */

void BtlHudAdviceUpdateStageHints(BtlHud *self, BtlBakugan *unit)
{
  switch (g_scriptGlobalVars[1]) {
  case 4:
    BtlHudAdviceTutorialIntro(self, 0x16);
    break;
  case 5:
    BtlHudAdviceStage5Hint(self, unit, 0x17);
    break;
  case 0x24:
    BtlHudAdviceStage36HintA(self, unit, 0x18);
    BtlHudAdviceStage36HintB(self, unit, 0x19);
    BtlHudAdviceStage36HintC(self, unit, 0x1a);
    break;
  default:
    break;
  }
}
