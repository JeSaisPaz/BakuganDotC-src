// bdc 0x0895e37c UiEquipMoveGridCursor
#include "bdc.h"

/* Moves the Bakugan-grid cursor of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`) on the repeat pad bits: `gridCursor` indexes a 4-column grid of 20 cells.
   Up (0x10) moves up a row, and from the top row selects the random-pick button (`onRandom` = 1);
   down (0x40) moves down a row, and from the bottom row selects the button; up/down from the button
   return to the bottom/top row in the cursor's column. Left (0x80) / right (0x20) wrap inside the row
   and are ignored on the button. Returns 1 when a direction was handled, else 0. */

s32 UiEquipMoveGridCursor(UiEquip *self)
{
  PadState *pad;
  s8 cursor;

  pad = self->base.pad;
  if ((s8)pad->repeat & 0x10) {
    if (self->onRandom != 0) {
      self->gridCursor = self->gridCursor % 4 + 16;
      self->onRandom = 0;
      return 1;
    }
    cursor = self->gridCursor;
    if (cursor < 4) {
      self->onRandom = 1;
      return 1;
    }
    self->gridCursor = cursor - 4;
    return 1;
  }
  if ((s8)pad->repeat & 0x40) {
    if (self->onRandom != 0) {
      self->gridCursor = self->gridCursor % 4;
      self->onRandom = 0;
      return 1;
    }
    cursor = self->gridCursor;
    if (cursor < 16) {
      self->gridCursor = cursor + 4;
      return 1;
    }
    self->onRandom = 1;
    return 1;
  }
  if ((s8)pad->repeat & 0x80) {
    if (self->onRandom != 0) {
      return 0;
    }
    cursor = self->gridCursor;
    if (cursor % 4 == 0) {
      self->gridCursor = cursor + 3;
      return 1;
    }
    self->gridCursor = cursor - 1;
    return 1;
  }
  if ((s8)pad->repeat & 0x20) {
    if (self->onRandom != 0) {
      return 0;
    }
    cursor = self->gridCursor;
    if (cursor % 4 == 3) {
      self->gridCursor = cursor - 3;
      return 1;
    }
    self->gridCursor = cursor + 1;
    return 1;
  }
  return 0;
}
