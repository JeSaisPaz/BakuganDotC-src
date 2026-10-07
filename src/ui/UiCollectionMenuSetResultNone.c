// bdc 0x089748c8 UiCollectionMenuSetResultNone
#include "bdc.h"

/* Sets menu result 0 for `UiCollectionMenu` (`UiSetMenuResult`) when it
   exits. */

void UiCollectionMenuSetResultNone(UiCollectionMenu *self)

{
  UiSetMenuResult(&self->base,0);
  return;
}

