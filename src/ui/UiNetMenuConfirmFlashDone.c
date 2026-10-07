// bdc 0x0894ef0c UiNetMenuConfirmFlashDone
#include "bdc.h"

/* Advances flash channel 0 (`UiFlashStep(0)`) and returns 1 once it has finished, else 0. */

s32 UiNetMenuConfirmFlashDone(void)

{
  if (UiFlashStep(0) != 0) {
    return 1;
  }
  return 0;
}
