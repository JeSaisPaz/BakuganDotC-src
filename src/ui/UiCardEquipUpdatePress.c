// bdc 0x0896e450 UiCardEquipUpdatePress
#include "bdc.h"

/* Plays the press animation of `UiCardEquip`. Outside the gauge row (`row != 2`)
   it only steps flash slot 0 and returns 1 once `UiFlashStep` reports it done, else 0. In the gauge
   row it runs `UiCardEquipUpdateGaugeRow` and steps the flash; the pressed arrow triple of the
   selected Bakugan (group 14 + `selBakugan`*6 + `gaugeDir`*3) is then either restored (flash done:
   third sprite tint white, all three scaled to 1, angle 0; returns 1) or grown by 0.1 per frame with
   the third sprite tinted 0.3 grey (returns 0). */

/* Index of the first sprite of the pressed arrow triple; the fields are re-read on every use. */
static inline int UiCardEquipPressArrow(UiCardEquip *self)
{
  return self->groups[14][0] + self->selBakugan * 6 + (u8)self->gaugeDir * 3;
}

int UiCardEquipUpdatePress(UiCardEquip *self)
{
  GfxSprite *sprite;
  int done;
  int i;

  if (self->row != 2) {
    if (UiFlashStep(0) != 0) {
      return 1;
    }
    return 0;
  }

  UiCardEquipUpdateGaugeRow(self);
  done = UiFlashStep(0);
  sprite = ((GfxSprite **)self->base.data)[UiCardEquipPressArrow(self) + 2];
  if (done != 0) {
    sprite->tint[0] = 1.0f;
    sprite->tint[1] = 1.0f;
    sprite->tint[2] = 1.0f;
    sprite->alpha = 1.0f;
    for (i = 0; i < 3; i++) {
      ((GfxSprite **)self->base.data)[UiCardEquipPressArrow(self) + i]->scaleX = 1.0f;
      sprite = ((GfxSprite **)self->base.data)[UiCardEquipPressArrow(self) + i];
      sprite->scaleY = sprite->scaleX;
      ((GfxSprite **)self->base.data)[UiCardEquipPressArrow(self) + i]->angle = 0.0f;
      sprite = ((GfxSprite **)self->base.data)[UiCardEquipPressArrow(self) + i];
      GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
    }
    return 1;
  }

  sprite->alpha = 1.0f;
  sprite->tint[0] = 0.3f;
  sprite->tint[1] = 0.3f;
  sprite->tint[2] = 0.3f;
  for (i = 0; i < 3; i++) {
    ((GfxSprite **)self->base.data)[UiCardEquipPressArrow(self) + i]->scaleX += 0.1f;
    sprite = ((GfxSprite **)self->base.data)[UiCardEquipPressArrow(self) + i];
    sprite->scaleY = sprite->scaleX;
    ((GfxSprite **)self->base.data)[UiCardEquipPressArrow(self) + i]->angle = 0.0f;
    sprite = ((GfxSprite **)self->base.data)[UiCardEquipPressArrow(self) + i];
    GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
  }
  return 0;
}
