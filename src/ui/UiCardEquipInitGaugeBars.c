// bdc 0x0896bfa0 UiCardEquipInitGaugeBars
#include "bdc.h"

/* Initialises the G-power gauge sprites (group 15, two per Bakugan) of
   `UiCardEquip`: the even sprite is the bar, whose UV rectangle and width are set
   to `value*62/150 + 1` pixels (`GfxSpriteSetUvRectXYWH`, `UiSpriteSetSize`); the odd sprite is
   its frame. */

void UiCardEquipInitGaugeBars(UiCardEquip *self)
{
  int first = self->groups[15][0];
  int i;
  float rect[4];

  for (i = first; i < self->groups[15][0] + (s8)self->groups[15][1]; i++) {
    int slot = i - first;
    GfxSprite *sprite;
    int width;

    if (slot >= self->bakuganCount * 2)
      continue;
    sprite = ((GfxSprite **)self->base.data)[i];
    if ((slot & 1) == 0) {
      /* Bar: visible, linear filtering, unit scale, width from the gauge value. */
      sprite->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->flags |= 0x20;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
      ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
      width = self->gauge[(i - self->groups[15][0]) / 2] * 62 / 150 + 1;
      rect[0] = 0.0f;
      rect[1] = 0.0f;
      rect[2] = (float)width;
      rect[3] = 16.0f;
      GfxSpriteSetUvRectXYWH(((GfxSprite **)self->base.data)[i], rect);
      width = self->gauge[(i - self->groups[15][0]) / 2] * 62 / 150 + 1;
      UiSpriteSetSize((float)width, 16.0f, ((GfxSprite **)self->base.data)[i]);
    } else {
      /* Frame. */
      UiCardEquipShowSprite(self, sprite);
      ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
    }
  }
}
