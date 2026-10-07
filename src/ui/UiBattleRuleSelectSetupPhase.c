// bdc 0x08952a30 UiBattleRuleSelectSetupPhase
#include "bdc.h"

/* Phase 1 of `UiBattleRuleSelect` (phase table `0x08a9d4e0`): step 0
   builds the screen (`UiBattleRuleSelectBuildSprites`); step 1 enters the offline main phase 2
   (`UiBattleRuleSelectMainPhase`) or, in network mode (`SaveGetProfileFlag0`), phase 4
   (`UiBattleRuleSelectNetPhase`). */

void UiBattleRuleSelectSetupPhase(UiBattleRuleSelect *self)

{
  if ((self->base).phaseStep == 0) {
    UiBattleRuleSelectBuildSprites(self);
    (self->base).phaseStep = (self->base).phaseStep + 1;
  }
  else {
    if (SaveGetProfileFlag0() == 0) {
      (self->base).phaseStep = 0;
      (self->base).phase = (self->base).phase + 1;
    }
    else {
      (self->base).phase = 4;
      (self->base).phaseStep = 0;
    }
  }
  return;
}

