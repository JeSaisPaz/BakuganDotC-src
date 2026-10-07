// bdc 0x0897bea8 UiCollectionSphereChangePage
#include "bdc.h"

/* Picks the next page of the sphere (Bakugan figure) collection screen
   (`UiCollectionSphere`) when left/right repeats at the grid edge:
   copies `page` into `oldPage`, then steps `oldPage` to the target page and sets `pageDir`
   (1 = left, 2 = right). Categories 0/1 wrap right over `(entryCount+5)/6` pages and stop at
   page 0 on the left; category 2 pages are stepped by `UiCollectionSphereGetPageKind`
   (special-item pages, kind 0: only from the grid edge column; model pages, kinds 1/2: from
   any column; right stops at page 5). Returns 1 when a target page was set, else 0 (other
   categories, no button, not at the edge). */

s32 UiCollectionSphereChangePage(UiCollectionSphere *self)
{
  s8 page = self->page;
  s8 category = self->category;
  PadState *pad;
  s32 lastPage;

  self->oldPage = page;
  if (category < 2) {
    if (category < 0) {
      return 0;
    }
    pad = self->base.pad;
    if (pad->repeat & 0x80) {
      if (self->cursor % 3 != 0 || self->oldPage == 0) {
        return 0;
      }
      self->oldPage = self->oldPage - 1;
      if (self->oldPage < 0) {
        self->oldPage = (self->entryCount + 5) / 6 - 1;
      }
      self->pageDir = 1;
      return 1;
    }
    if ((pad->repeat & 0x20) == 0) {
      return 0;
    }
    lastPage = (self->entryCount + 5) / 6 - 1;
    if (page == lastPage) {
      return 0;
    }
    if (self->cursor % 3 != 2 && self->cursor + page * 6 + 1 < self->entryCount) {
      return 0;
    }
    self->oldPage = self->oldPage + 1;
    if (lastPage < self->oldPage) {
      self->oldPage = 0;
    }
    self->pageDir = 2;
    return 1;
  }
  if (category >= 3) {
    return 0;
  }
  if (UiCollectionSphereGetPageKind(self, (u8)page) != 0) {
    pad = self->base.pad;
    if (pad->repeat & 0x80) {
      if (self->oldPage == 0) {
        return 0;
      }
      self->oldPage = self->oldPage - 1;
      self->pageDir = 1;
      return 1;
    }
    if ((pad->repeat & 0x20) == 0 || self->oldPage == 5) {
      return 0;
    }
    self->oldPage = self->oldPage + 1;
    self->pageDir = 2;
    return 1;
  }
  pad = self->base.pad;
  if (pad->repeat & 0x80) {
    if (self->cursor % 3 != 0 || self->oldPage == 0) {
      return 0;
    }
    self->oldPage = self->oldPage - 1;
    self->pageDir = 1;
    return 1;
  }
  if ((pad->repeat & 0x20) == 0 || self->cursor % 3 != 2) {
    return 0;
  }
  self->pageDir = 2;
  self->oldPage = self->oldPage + 1;
  return 1;
}
