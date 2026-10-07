// bdc 0x0899bb10 UiWorldMapSelectFlashDone
#include "bdc.h"

/* Advances UI flash slot 0 (`UiFlashStep(0)`) for `UiWorldMap`; returns 1 when
   the flash started by `UiWorldMapStartSelectFlash` has finished. */

int UiWorldMapSelectFlashDone(UiScreen *screen)
{
  if (UiFlashStep(0) != 0) {
    return 1;
  }
  return 0;
}
