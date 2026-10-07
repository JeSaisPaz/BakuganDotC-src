// bdc 0x08984828 UiCollectionCardChangePage
#include "bdc.h"

/* Changes the target page `+0xbce` of `UiCollectionCard` when Left/Right is
   pressed at the grid edge (pages 0..0x13, no wrap), recording the direction in `+0xbcf` (1 left, 2
   right); a fresh press resets fast scrolling (`UiCollectionCardResetFastScroll`). Returns 1 when
   the page changes. */

int UiCollectionCardChangePage(UiCollectionCard *self)

{
  PadState *pad = self->base.pad;

  self->targetPage = self->page;
  if ((pad->repeat & 0x80) == 0) {
    if ((pad->repeat & 0x20) != 0 && self->page != 0x13) {
      if (self->cursor % 2 == 1) {
        self->targetPage = self->targetPage + 1;
        self->pageDir = 2;
        if ((pad->pressed & 0x20) != 0) {
          UiCollectionCardResetFastScroll(self);
        }
        return 1;
      }
    }
  } else {
    if (self->cursor % 2 == 0 && self->targetPage != 0) {
      self->targetPage = self->targetPage - 1;
      self->pageDir = 1;
      if ((pad->pressed & 0x80) != 0) {
        UiCollectionCardResetFastScroll(self);
      }
      return 1;
    }
  }
  return 0;
}
