// bdc 0x0891954c UiAdvSelectUpdateArrowFlash
#include "bdc.h"

/* While `arrowFlashOn` is set, lights the two arrow sprites (slots 0x1a/0x1b of the screen's
   sprite table, data `+0x68`) of the adventure partner-select screen (`UiAdvSelectCtor`,
   task 376) from a 4-step on/off pattern (`UiAdvSelectSetArrowLit`), advancing the step
   every 8 frames (`arrowFlashTimer`, `arrowFlashStep`). */

typedef struct AdvSelectSprites {
  u8 unk00[0x68];
  GfxSprite *sprite[2];
} AdvSelectSprites;

void UiAdvSelectUpdateArrowFlash(UiAdvSelect *self)
{
  u8 pattern[8] = {0, 0, 1, 0, 1, 1, 0, 1};
  s32 i;
  AdvSelectSprites *sprites = (AdvSelectSprites *)self->base.data;

  if (self->arrowFlashOn != 0) {
    for (i = 0x1a; i < 0x1c; i++) {
      UiAdvSelectSetArrowLit(self, sprites->sprite[i - 0x1a],
                             pattern[self->arrowFlashStep * 2 + (i - 0x1a)]);
    }
    if ((float)self->arrowFlashTimer == 8.0f) {
      self->arrowFlashTimer = 0;
      self->arrowFlashStep = (self->arrowFlashStep + 1) & 3;
    } else {
      self->arrowFlashTimer = self->arrowFlashTimer + 1;
    }
  }
}
