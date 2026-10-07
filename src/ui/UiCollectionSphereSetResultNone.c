// bdc 0x0897a200 UiCollectionSphereSetResultNone
#include "bdc.h"

/* Sets menu result 0 for `UiCollectionSphere` (`UiSetMenuResult`) when
   it exits. */

void UiCollectionSphereSetResultNone(UiCollectionSphere *self)

{
  UiSetMenuResult(&self->base,0);
  return;
}

