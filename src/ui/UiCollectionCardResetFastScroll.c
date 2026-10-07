// bdc 0x08984814 UiCollectionCardResetFastScroll
#include "bdc.h"

/* Resets the fast page-scroll state of `UiCollectionCard` (`+0xcd4` off,
   frame counter `+0xcd5` = 5, flip counter `+0xcd6` = 0). */

void UiCollectionCardResetFastScroll(UiCollectionCard *self)

{
  self->scrollFrames = '\x05';
  self->fastScroll = '\0';
  self->flipCount = '\0';
  return;
}

