// bdc 0x08975994 UiCollectionMenuConfirmEntry
#include "bdc.h"

/* Picks the step after a confirmed entry of `UiCollectionMenu`: main entries
   0 and 2 switch to their sub-page (step 0xb), entries 1, 3, 4 and every sub-page entry open their
   screen (step 0xd); hides the cursor first. */

void UiCollectionMenuConfirmEntry(UiCollectionMenu *self)

{
  UiCollectionMenuHideCursor(self);
  if (self->page == 0) {
    switch ((u8)self->selMain) {
    case 1:
    case 3:
    case 4:
      self->base.phaseStep = 0xd;
      return;
    case 0:
    case 2:
      self->selSub = 0;
      self->base.phaseStep = 0xb;
      return;
    default:
      return;
    }
  }
  self->base.phaseStep = 0xd;
}
