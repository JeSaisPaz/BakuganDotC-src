// bdc 0x08982f48 UiCollectionCardSetResultNone
#include "bdc.h"

/* Sets menu result 0 for `UiCollectionCard` (`UiSetMenuResult`) when it
   exits. */

void UiCollectionCardSetResultNone(UiCollectionCard *self)

{
  UiSetMenuResult(&self->base,0);
  return;
}

