// bdc 0x0898ded0 UiCollectionFigureChangePage
#include "bdc.h"

/* Handles a page change of `UiCollectionFigure`. Always copies `page`
   into `targetPage` first; then, in category 0 only: repeat-left (0x80) on the left column steps
   `targetPage` back (sets `pageDir` 1), repeat-right (0x20) on the right column or the last entry,
   when not already on the last of the `(entryCount + 5) / 6` pages, steps it forward, wrapping to
   0 (sets `pageDir` 2). Returns 1 when the page changes, else 0. */

int UiCollectionFigureChangePage(UiCollectionFigure *self)

{
  s8 page;
  PadState *pad;
  int count;
  int pages;

  page = self->page;
  self->targetPage = page;
  if (self->category > 0 || self->category < 0) {
    return 0;
  }
  pad = self->base.pad;
  if ((s8)pad->repeat & 0x80) {
    if (self->cursor % 3 != 0) {
      return 0;
    }
    if (self->targetPage == 0) {
      return 0;
    }
    self->targetPage = self->targetPage - 1;
    if (self->targetPage < 0) {
      self->targetPage = (self->entryCount + 5) / 6 - 1;
    }
    self->pageDir = 1;
    return 1;
  }
  if (((s8)pad->repeat & 0x20) == 0) {
    return 0;
  }
  count = self->entryCount;
  pages = (count + 5) / 6;
  if (page == pages - 1) {
    return 0;
  }
  if (self->cursor % 3 != 2) {
    if (self->cursor + page * 6 + 1 < count) {
      return 0;
    }
  }
  self->targetPage = self->targetPage + 1;
  if (pages - 1 < self->targetPage) {
    self->targetPage = 0;
  }
  self->pageDir = 2;
  return 1;
}
