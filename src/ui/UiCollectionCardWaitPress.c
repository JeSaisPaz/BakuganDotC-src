// bdc 0x08984c80 UiCollectionCardWaitPress
#include "bdc.h"

/* Returns 1 once the shared press animation (`UiFlashStep(0)`) of
   `UiCollectionCard` has finished. */

int UiCollectionCardWaitPress(UiCollectionCard *self)

{
  if (UiFlashStep(0) != 0) {
    return 1;
  }
  return 0;
}
