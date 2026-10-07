// bdc 0x0897595c UiCollectionMenuWaitPress
#include "bdc.h"

/* Returns 1 once the shared press animation (`UiFlashStep(0)`) of
   `UiCollectionMenu` has finished. */

int UiCollectionMenuWaitPress(UiCollectionMenu *self)

{
  if (UiFlashStep(0) != 0) {
    return 1;
  }
  return 0;
}
