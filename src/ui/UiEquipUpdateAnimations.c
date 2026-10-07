// bdc 0x0895ac2c UiEquipUpdateAnimations
#include "bdc.h"

/* Per-frame animations of `UiEquip` selected by the bits of `animFlags` (+0x4ce1):
   bit 0 spins the emblem sprite `spriteIdx[11]` (+0x5176) by 0.5° per frame
   (`GfxSpriteSetScaleRotation`); bit 1 updates the gauge of each player whose `scrollFast` is not -1
   (`UiEquipUpdatePlayerGauge`); bit 2 animates the Bakugan name plates
   (`UiEquipAnimateBakuganPlates`). */

void UiEquipUpdateAnimations(UiEquip *self)
{
  GfxSprite *sprite;
  int player;

  if ((self->animFlags & 1) != 0) {
    sprite = ((GfxSprite **)self->base.data)[self->spriteIdx[11]];
    sprite->angle = sprite->angle + 0.008726646f;
    sprite = ((GfxSprite **)self->base.data)[self->spriteIdx[11]];
    GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
  }
  if ((self->animFlags & 2) != 0) {
    for (player = 0; player < 4; player++) {
      if (self->gaugeScroll[player].scrollFast != -1.0f) {
        UiEquipUpdatePlayerGauge(self, (u8)player);
      }
    }
  }
  if ((self->animFlags & 4) != 0) {
    UiEquipAnimateBakuganPlates(self);
  }
}
