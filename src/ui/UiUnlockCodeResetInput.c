// bdc 0x08993820 UiUnlockCodeResetInput
#include "bdc.h"

/* Clears the input state of `UiUnlockCode`: caret `+0x78`, key column/row
   `+0x7c/+0x80`, character page `+0x84/+0x89`, flags and timers, sets the `+0x104` digit slots
   `+0xb4` to −1 and clears the comparison buffer `+0xdc`; in 10-digit mode shows sprite 0x13
   (data `+0x4c`). */

void UiUnlockCodeResetInput(UiUnlockCode *self)
{
  int i;

  self->caret = 0;
  self->keyColumn = 0;
  self->keyRow = 0;
  self->pageOffset = 0;
  self->inputFlag = '\0';
  self->page = '\0';
  self->introTimer = 0;
  self->inputCounter = 0;
  self->cursorAngle = 0;
  self->keyPopOn = '\0';
  self->keyPopTimer = 0;
  self->okFlashT = 0.0f;
  for (i = 0; i < self->digitCount; i++) {
    self->entered[i] = -1;
  }
  memset(self->normalized, 0, 0x28);
  if (self->dimensionsMode == 1) {
    ((GfxSprite **)self->base.data)[0x13]->flags |= 1;
  }
}
