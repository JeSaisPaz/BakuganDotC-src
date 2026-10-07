// bdc 0x0897c7d4 UiCollectionSphereClearDetailModel
#include "bdc.h"

/* Clears the active flag of the detail model slot (`+0x12c0[+0x1318]`) of
   `UiCollectionSphere`. */

void UiCollectionSphereClearDetailModel(UiCollectionSphere *self)

{
  self->cellActive[self->detailCell] = '\0';
  return;
}

