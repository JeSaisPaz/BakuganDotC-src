// bdc 0x0891a284 UiAdvSelectTweenNamePanels
#include "bdc.h"

/* Starts the tweens of the two Bakugan name panels of the adventure partner-select screen.
   Clears and stores `dir`/`hide` (namePanelsDir/namePanelsHide). When showing (hide == 0), for
   panel i in 0..1 (0 = the cursor candidate's Bakugan, 1 = its partner): sets the attribute-row
   sprite 28+i (panel 0 is also flipped horizontally), places it at spritePos[28+i].x -+ 32, makes it
   visible with alpha 0 and slides it by +-32 px from scale 1.2 (tween 28+i); then sets the name
   sprite 32+i (panel 1 shows name id 0) and the attribute-cell sprite 30+i, both copying the panel's
   alpha, offset from it by namePanelOffset scaled by the panel's scale, and given its scale. When
   hiding, just starts a fade-out tween (from the current X scale) on sprites 28 and 29. */

#define ADV_SPRITE(n) (((GfxSprite **)self->base.data)[n])

void UiAdvSelectTweenNamePanels(UiAdvSelect *self, u8 dir, u8 hide)
{
  int i;
  float slideTo;

  memset(&self->namePanelsDir, 0, 4);
  self->namePanelsHide = hide;
  self->namePanelsDir = dir;
  if (self->namePanelsHide == 0) {
    for (i = 0; i < 2; i++) {
      if (i == 0) {
        UiAdvSelectSetAttributeRow(self, ADV_SPRITE(28 + i), self->candidates[self->cursor].bakugan);
        GfxSpriteFlipU(ADV_SPRITE(28 + i));
        slideTo = 32.0f;
        ADV_SPRITE(28 + i)->posX = self->spritePos[28 + i][0] - 32.0f;
      } else {
        UiAdvSelectSetAttributeRow(self, ADV_SPRITE(28 + i), self->candidates[self->cursor].partner);
        slideTo = -32.0f;
        ADV_SPRITE(28 + i)->posX = self->spritePos[28 + i][0] + 32.0f;
      }
      ADV_SPRITE(28 + i)->flags |= 1;
      ADV_SPRITE(28 + i)->alpha = 0.0f;
      UiTweenBeginSlide(1.2f, 0.0f, slideTo, hide, ADV_SPRITE(28 + i), &self->tweens[28 + i], 7);

      if (i == 0) {
        UiAdvSelectSetBakuganName(self, ADV_SPRITE(32 + i), self->candidates[self->cursor].bakugan);
      } else {
        UiAdvSelectSetBakuganName(self, ADV_SPRITE(32 + i), 0);
      }
      ADV_SPRITE(32 + i)->flags |= 1;
      ADV_SPRITE(32 + i)->alpha = ADV_SPRITE(28 + i)->alpha;
      ADV_SPRITE(32 + i)->posX =
          ADV_SPRITE(28 + i)->posX + self->namePanelOffset[i * 4 + 0] * ADV_SPRITE(28 + i)->scaleX;
      ADV_SPRITE(32 + i)->posY =
          ADV_SPRITE(28 + i)->posY + self->namePanelOffset[i * 4 + 1] * ADV_SPRITE(28 + i)->scaleY;
      ADV_SPRITE(32 + i)->flags |= 0x20;
      UiSpriteSetScaleRotation(ADV_SPRITE(32 + i), ADV_SPRITE(28 + i)->scaleX,
                               ADV_SPRITE(28 + i)->scaleY, 0.0f);

      if (i == 0) {
        UiAdvSelectSetAttributeCell(self, ADV_SPRITE(30 + i), self->candidates[self->cursor].bakugan);
      } else {
        UiAdvSelectSetAttributeCell(self, ADV_SPRITE(30 + i), self->candidates[self->cursor].partner);
      }
      ADV_SPRITE(30 + i)->flags |= 1;
      ADV_SPRITE(30 + i)->alpha = ADV_SPRITE(28 + i)->alpha;
      ADV_SPRITE(30 + i)->posX =
          ADV_SPRITE(28 + i)->posX + self->namePanelOffset[i * 4 + 2] * ADV_SPRITE(28 + i)->scaleX;
      ADV_SPRITE(30 + i)->posY =
          ADV_SPRITE(28 + i)->posY + self->namePanelOffset[i * 4 + 3] * ADV_SPRITE(28 + i)->scaleY;
      ADV_SPRITE(30 + i)->flags |= 0x20;
      UiSpriteSetScaleRotation(ADV_SPRITE(30 + i), ADV_SPRITE(28 + i)->scaleX,
                               ADV_SPRITE(28 + i)->scaleY, 0.0f);
    }
  } else {
    for (i = 0; i < 2; i++) {
      UiTweenBegin(ADV_SPRITE(28 + i)->scaleX, hide, ADV_SPRITE(28 + i), &self->tweens[28 + i], 1);
    }
  }
}

#undef ADV_SPRITE
