// bdc 0x0898a020 UiCollectionTheaterWaitPress
#include "bdc.h"

/* Returns 1 once the shared press animation (`UiFlashStep(0)`) of
   `UiCollectionTheater` has finished. */

int UiCollectionTheaterWaitPress(UiScreen *screen)

{
  if (UiFlashStep(0) != 0) {
    return 1;
  }
  return 0;
}
