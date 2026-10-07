// bdc 0x08976518 UiCollectionMenuStartLightBlink
#include "bdc.h"

/* Enables the item-box light blink of `UiCollectionMenu` (`+0x754`) and
   forces a refresh (`+0x502` = 100). */

void UiCollectionMenuStartLightBlink(UiCollectionMenu *self)

{
  memset(&self->lightBlink,0,0xc);
  self->lightBlink = '\x01';
  self->refresh = 'd';
  return;
}

