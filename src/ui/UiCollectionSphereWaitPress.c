// bdc 0x0897c6e4 UiCollectionSphereWaitPress
#include "bdc.h"

/* Returns 1 once the shared press animation (`UiFlashStep(0)`) of
   `UiCollectionSphere` has finished. */

int UiCollectionSphereWaitPress(UiCollectionSphere *self)

{
  if (UiFlashStep(0) != 0) {
    return 1;
  }
  return 0;
}
