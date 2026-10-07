// bdc 0x0891a808 UiAdvSelectFlashDone
#include "bdc.h"

/* Advances both flash channels (`UiFlashStep(1)`, `UiFlashStep(0)`) and returns 1 once channel 0
   has completed, 0 while it is still running. */

int UiAdvSelectFlashDone(void)

{
  UiFlashStep(1);
  if (UiFlashStep(0) == 0) {
    return 0;
  }
  return 1;
}
