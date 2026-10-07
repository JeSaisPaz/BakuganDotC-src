// bdc 0x08971ca4 UiOptionWaitPress
#include "bdc.h"

/* Returns 1 once the shared press animation (`UiFlashStep(0)`) of `UiOption` has
   finished. */

int UiOptionWaitPress(UiOption *self)

{
  if (UiFlashStep(0) != 0) {
    return 1;
  }
  return 0;
}
