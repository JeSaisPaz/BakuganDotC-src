// bdc 0x08952a9c UiBattleRuleSelectExitPhase
#include "bdc.h"

/* Phase 3 (table `0x08a9d4d8` entry 4) of the battle-rule select screen: requests the screen's
   close (`closeRequested`, `+0x4c` = 1). */

void UiBattleRuleSelectExitPhase(UiBattleRuleSelect *self)

{
  (self->base).closeRequested = '\x01';
  return;
}

