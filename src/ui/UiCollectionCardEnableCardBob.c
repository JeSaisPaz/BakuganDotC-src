// bdc 0x08984454 UiCollectionCardEnableCardBob
#include "bdc.h"

/* Resets the card bob record of `UiCollectionCard` (`+0xc78`, 0x0c bytes)
   and enables (1) or disables it. */

void UiCollectionCardEnableCardBob(UiCollectionCard *self, u8 enable)

{
  memset(&self->bobOn,0,0xc);
  self->bobOn = enable;
  return;
}

