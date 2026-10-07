// bdc 0x089854e0 UiCollectionCardUpdateFastScrollTimer
#include "bdc.h"

/* Counts frames since the last page flip of `UiCollectionCard` (`+0xcd5`, up
   to 5); after 5 idle frames fast scrolling is turned off. */

void UiCollectionCardUpdateFastScrollTimer(UiCollectionCard *self)

{
  if ((float)self->scrollFrames < 5.0f) {
    self->scrollFrames = self->scrollFrames + 1;
    return;
  }
  self->fastScroll = '\0';
  self->flipCount = '\0';
  return;
}

