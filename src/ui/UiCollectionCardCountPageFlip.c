// bdc 0x0898551c UiCollectionCardCountPageFlip
#include "bdc.h"

/* Counts a page flip of `UiCollectionCard`: restarts the idle counter and,
   after four quick flips in a row (`+0xcd6`), turns fast scrolling on (`+0xcd4`). */

void UiCollectionCardCountPageFlip(UiCollectionCard *self)

{
  self->scrollFrames = '\0';
  if (3 < self->flipCount) {
    self->fastScroll = '\x01';
    return;
  }
  self->flipCount = self->flipCount + 1;
  return;
}

