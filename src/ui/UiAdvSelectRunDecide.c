// bdc 0x0891ad3c UiAdvSelectRunDecide
#include "bdc.h"

/* Decision sequence of the adventure partner-select screen, one `decideStep` per frame:
   0: hides sprite 37, sets sprite 33 to the chosen candidate's partner name
   (`UiAdvSelectSetBakuganName`) and sprite 4 to its partner picture
   (`UiAdvSelectSetBakuganPicture`), shows sprites 1 and 4, starts the appear tweens of sprites
   33, 1 and 4 (`UiTweenBegin`) and plays sound 10;
   1: advances those tweens over 32 frames (`UiTweenUpdate`) until one completes;
   2: hides sprites 1 and 4, sets sprite 3 to the partner picture, shows the cursor's sprite
   18+cursor at full alpha and evolves the chosen Bakugan in the save (`UiBakuganEvolve`);
   3: starts 8-frame flashes on sprites 0 (slot 0) and 3 (slot 1) (`UiFlashStart`);
   4: steps both flashes (`UiFlashStep`) until slot 0 completes;
   5: counts `decideTimer` up to 32, then sets step 6.
   Returns 0 while the sequence runs (steps 0..5), 1 once `decideStep` >= 6. */

s32 UiAdvSelectRunDecide(UiAdvSelect *self)
{
  u8 step = self->decideStep;
  u8 finished;

  if (step >= 6) {
    return 1;
  }
  if (step == 1) {
    finished = UiTweenUpdate(1.0f, 1.0f, 32.0f, 0, ((GfxSprite **)self->base.data)[33],
                             &self->tweens[33], 1);
    finished += UiTweenUpdate(2.0f, 1.0f, 32.0f, 0, ((GfxSprite **)self->base.data)[1],
                              &self->tweens[1], 3);
    finished += UiTweenUpdate(2.0f, 1.0f, 32.0f, 0, ((GfxSprite **)self->base.data)[4],
                              &self->tweens[4], 3);
    if (finished != 0) {
      self->decideStep++;
    }
  } else if (step == 2) {
    ((GfxSprite **)self->base.data)[1]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[4]->flags &= ~1u;
    UiAdvSelectSetBakuganPicture(self, ((GfxSprite **)self->base.data)[3],
                                 self->candidates[self->cursor].partner, true);
    ((GfxSprite **)self->base.data)[18 + self->cursor]->flags |= 1u;
    ((GfxSprite **)self->base.data)[18 + self->cursor]->alpha = 1.0f;
    UiBakuganEvolve(self->candidates[self->cursor].bakugan);
    self->decideStep++;
  } else if (step == 3) {
    UiFlashStart(8.0f, ((GfxSprite **)self->base.data)[0], 0, 0);
    UiFlashStart(8.0f, ((GfxSprite **)self->base.data)[3], 0, 1);
    self->decideStep++;
  } else if (step == 4) {
    UiFlashStep(1);
    if (UiFlashStep(0) != 0) {
      self->decideStep++;
    }
  } else if (step == 5) {
    if (self->decideTimer == 32) {
      self->decideStep = 6;
    } else {
      self->decideTimer++;
    }
  } else {
    ((GfxSprite **)self->base.data)[37]->flags &= ~1u;
    UiAdvSelectSetBakuganName(self, ((GfxSprite **)self->base.data)[33],
                              self->candidates[self->cursor].partner);
    ((GfxSprite **)self->base.data)[33]->alpha = 0.0f;
    UiTweenBegin(1.0f, 0, ((GfxSprite **)self->base.data)[33], &self->tweens[33], 1);
    ((GfxSprite **)self->base.data)[1]->flags |= 1u;
    ((GfxSprite **)self->base.data)[1]->alpha = 0.0f;
    UiTweenBegin(2.0f, 0, ((GfxSprite **)self->base.data)[1], &self->tweens[1], 3);
    UiAdvSelectSetBakuganPicture(self, ((GfxSprite **)self->base.data)[4],
                                 self->candidates[self->cursor].partner, true);
    ((GfxSprite **)self->base.data)[4]->flags |= 1u;
    ((GfxSprite **)self->base.data)[4]->alpha = 0.0f;
    UiTweenBegin(2.0f, 0, ((GfxSprite **)self->base.data)[4], &self->tweens[4], 3);
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 10, 0, 0);
    }
    self->decideStep++;
  }
  return 0;
}
