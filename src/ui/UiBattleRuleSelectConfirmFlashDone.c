// bdc 0x089536a4 UiBattleRuleSelectConfirmFlashDone
#include "bdc.h"

/* Advances flash channel 0 (`UiFlashStep(0)`) and returns 1 once it has finished, else 0. */

s32 UiBattleRuleSelectConfirmFlashDone(void)

{
  
  if (UiFlashStep(0)) {
    return 1;
  }
  return 0;
}

