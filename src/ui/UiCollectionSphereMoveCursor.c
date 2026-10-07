// bdc 0x0897bc04 UiCollectionSphereMoveCursor
#include "bdc.h"

/* Moves the cursor `+0xee0` of the sphere (Bakugan figure) collection screen (task 312,
   `maybe_UiScreen312Ctor`; 3x2 grid pages of collected Bakugan shown as 3D models
   (`"00_P_Dragonoid_N_P.gmo"`…), names `"cha_spherename_colle_%02d"`, pop-out motions
   (`"00_dor_dir_popout"`…); cursor `+0xee0`, page `+0xee1`, category `+0xee5`, entry lists
   `+0x1250`) inside the 3x2 grid of the current page with the repeated d-pad bits
   (0x10 up, 0x40 down, 0x80 left, 0x20 right). Categories 0/1 stay within the entry count
   `+0xeec` (down from the top row onto a missing cell jumps to the last entry of the bottom
   row when that row has at least one entry); category 2 only moves on special-item pages
   (page kind 0, see `UiCollectionSphereGetPageKind`) and ignores the count, never on the
   model pages (kinds 1/2); other categories never move. Returns 1 when it moved, else 0. */

s32 UiCollectionSphereMoveCursor(UiCollectionSphere *self)

{
  s8 category = self->category;
  u8 repeat;
  s32 cursor;
  s32 rest;

  if (category < 2) {
    if (category < 0) {
      return 0;
    }
    repeat = (u8)self->base.pad->repeat;
    if (repeat & 0x10) {
      cursor = self->cursor;
      if (cursor < 3) {
        return 0;
      }
      self->cursor = cursor - 3;
      return 1;
    }
    if (repeat & 0x40) {
      cursor = self->cursor;
      if (cursor >= 3) {
        return 0;
      }
      if (self->page * 6 + cursor + 3 < self->entryCount) {
        self->cursor = cursor + 3;
        return 1;
      }
      rest = (s32)self->entryCount % 6;
      if (rest < 4) {
        return 0;
      }
      self->cursor = rest - 1;
      return 1;
    }
    if (repeat & 0x80) {
      cursor = self->cursor;
      if (cursor % 3 == 0) {
        return 0;
      }
      self->cursor = cursor - 1;
      return 1;
    }
    if (repeat & 0x20) {
      cursor = self->cursor;
      if (cursor % 3 == 2) {
        return 0;
      }
      if (cursor + self->page * 6 + 1 >= self->entryCount) {
        return 0;
      }
      self->cursor = cursor + 1;
      return 1;
    }
    return 0;
  }

  if (category >= 3) {
    return 0;
  }
  if (UiCollectionSphereGetPageKind(self, (u8)self->page) != 0) {
    return 0;
  }
  repeat = (u8)self->base.pad->repeat;
  if (repeat & 0x10) {
    cursor = self->cursor;
    if (cursor < 3) {
      return 0;
    }
    self->cursor = cursor - 3;
    return 1;
  }
  if (repeat & 0x40) {
    cursor = self->cursor;
    if (cursor >= 3) {
      return 0;
    }
    self->cursor = cursor + 3;
    return 1;
  }
  if (repeat & 0x80) {
    cursor = self->cursor;
    if (cursor % 3 == 0) {
      return 0;
    }
    self->cursor = cursor - 1;
    return 1;
  }
  if (repeat & 0x20) {
    cursor = self->cursor;
    if (cursor % 3 == 2) {
      return 0;
    }
    self->cursor = cursor + 1;
    return 1;
  }
  return 0;
}
