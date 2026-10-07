// bdc 0x0892fe1c UiBakuganSelectTweenGrid
#include "bdc.h"

/* Starts the open (`closing` = 0) or close slide tweens of the Bakugan grid of
   `UiBakuganSelect`: four 20-sprite layers (sprites 26–45, 46–65, 66–85 and
   94–113, each with its own `tweens[i]`) laid out in rows of 10. On open each cell's sprite is put in
   draw layer 2, its visibility set (layer 26: always visible; layer 46: visible only for an empty
   entry; layer 66: visible for an occupied entry, given its chara picture and dimmed to 0.3 tint /
   0 alpha when the Bakugan is in `unselectableMask[0]`; layer 94: visible when `entries[j].notEvolved`),
   placed at its row's first cell X (`spritePos[row + layer]`; the 94 layer uses the 26 layer's
   positions plus `layerOffset[0]`, X and Y) and slid with `UiTweenBeginSlide` (mode 5) to its own
   cell X; on close the slide runs back from the cell X to the row start, fading out. */

void UiBakuganSelectTweenGrid(UiBakuganSelect *self, u8 closing)
{
  int i;
  int j;
  u8 row;
  float start;
  float off;
  GfxSprite *sprite;

  if (closing == 0) {
    for (i = 26; i < 46; i++) {
      j = i - 26;
      row = (j / 10) * 10;
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      start = self->spritePos[row + 26][0];
      ((GfxSprite **)self->base.data)[i]->posX = start;
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[i][0] - start, closing,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
    for (i = 46; i < 66; i++) {
      j = i - 46;
      row = (j / 10) * 10;
      sprite = ((GfxSprite **)self->base.data)[i];
      if (self->entries[j].bakugan != 0) {
        sprite->flags &= ~1u;
      } else {
        sprite->flags |= 1;
      }
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      start = self->spritePos[row + 46][0];
      ((GfxSprite **)self->base.data)[i]->posX = start;
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[i][0] - start, closing,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
    for (i = 66; i < 86; i++) {
      j = i - 66;
      row = (j / 10) * 10;
      sprite = ((GfxSprite **)self->base.data)[i];
      if (self->entries[j].bakugan != 0) {
        sprite->flags |= 1;
        UiBakuganSelectSetCharaPicture(self, ((GfxSprite **)self->base.data)[i],
                                       self->entries[j].bakugan);
        if ((self->unselectableMask[0] & (1u << self->entries[j].bakugan)) != 0) {
          sprite = ((GfxSprite **)self->base.data)[i];
          sprite->tint[0] = 0.3f;
          sprite->tint[1] = 0.3f;
          sprite->tint[2] = 0.3f;
          sprite->alpha = 0.0f;
        }
      } else {
        sprite->flags &= ~1u;
      }
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      start = self->spritePos[row + 66][0];
      ((GfxSprite **)self->base.data)[i]->posX = start;
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[i][0] - start, closing,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
    for (i = 94; i < 114; i++) {
      j = i - 94;
      row = (j / 10) * 10;
      sprite = ((GfxSprite **)self->base.data)[i];
      if (self->entries[j].notEvolved != 0) {
        sprite->flags |= 1;
      } else {
        sprite->flags &= ~1u;
      }
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      ((GfxSprite **)self->base.data)[i]->posX =
          self->spritePos[row + 26][0] + self->layerOffset[0][0];
      ((GfxSprite **)self->base.data)[i]->posY =
          self->spritePos[row + 26][1] + self->layerOffset[0][1];
      off = self->layerOffset[0][0];
      UiTweenBeginSlide(1.0f, 0.0f,
                        (self->spritePos[j + 26][0] + off) - (self->spritePos[row + 26][0] + off),
                        closing, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
  } else {
    for (i = 26; i < 46; i++) {
      row = ((i - 26) / 10) * 10;
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[row + 26][0] - self->spritePos[i][0], closing,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
    for (i = 46; i < 66; i++) {
      row = ((i - 46) / 10) * 10;
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[row + 46][0] - self->spritePos[i][0], closing,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
    for (i = 66; i < 86; i++) {
      row = ((i - 66) / 10) * 10;
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[row + 66][0] - self->spritePos[i][0], closing,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
    for (i = 94; i < 114; i++) {
      j = i - 94;
      row = (j / 10) * 10;
      off = self->layerOffset[0][0];
      UiTweenBeginSlide(1.0f, 0.0f,
                        (self->spritePos[row + 26][0] + off) - (self->spritePos[j + 26][0] + off),
                        closing, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
  }
}
