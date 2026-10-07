// bdc 0x08954288 UiBattleRuleSelectSubConfirmFlashDone
#include "bdc.h"

/* Advances flash channel 0 and returns 1 once it has finished, else 0. */

s32 UiBattleRuleSelectSubConfirmFlashDone(void)

{
  
  if (UiFlashStep(0)) {
    return 1;
  }
  return 0;
}

