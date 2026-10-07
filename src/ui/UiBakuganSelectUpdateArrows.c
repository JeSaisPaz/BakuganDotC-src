// bdc 0x0892e85c UiBakuganSelectUpdateArrows
#include "bdc.h"

/* While `arrowsOn` is set, lights the twelve arrow sprites 0x72..0x7d of the Bakugan select screen
   (`UiBakuganSelectCtor`), two per entry of the first six owned entries, from a 4-step blink
   pattern (`UiBakuganSelectSetArrowLit`); only arrows whose entry's Bakugan has its bit set in
   `unselectableMask[1]` are touched. The pattern step advances (mod 4) every 9 frames: the timer
   counts 0..8 and resets when it reads 8. */

void UiBakuganSelectUpdateArrows(UiBakuganSelect *self)
{
  u8 pattern[8];
  int i;

  pattern[0] = 0;
  pattern[1] = 0;
  pattern[2] = 1;
  pattern[3] = 0;
  pattern[4] = 1;
  pattern[5] = 1;
  pattern[6] = 0;
  pattern[7] = 1;
  if (self->arrowsOn != 0) {
    for (i = 0; i < 12; i++) {
      if ((self->unselectableMask[1] & (1u << (self->entries[(u8)(i / 2)].bakugan & 0x1f))) != 0) {
        UiBakuganSelectSetArrowLit(self, ((GfxSprite **)self->base.data)[0x72 + i],
                                   pattern[self->arrowsStep * 2 + i % 2]);
      }
    }
    if ((float)self->arrowsTimer == 8.0f) {
      self->arrowsTimer = 0;
      self->arrowsStep = (self->arrowsStep + 1) & 3;
    } else {
      self->arrowsTimer = self->arrowsTimer + 1;
    }
  }
}
