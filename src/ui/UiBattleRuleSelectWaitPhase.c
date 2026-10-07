// bdc 0x08952798 UiBattleRuleSelectWaitPhase
#include "bdc.h"

/* Phase 0 (table `0x08a9d4d8` entry 1) of the battle-rule select screen: waits one frame (`+0x2c`),
   then advances to phase 1 (`UiBattleRuleSelectSetupPhase`). */

void UiBattleRuleSelectWaitPhase(UiBattleRuleSelect *self)

{
  if ((self->base).phaseStep == 0) {
    (self->base).phaseStep = 1;
    return;
  }
  (self->base).phaseStep = 0;
  (self->base).phase = (self->base).phase + 1;
  return;
}

