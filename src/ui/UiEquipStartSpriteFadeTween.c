// bdc 0x089600c0 UiEquipStartSpriteFadeTween
#include "bdc.h"

/* Starts the fade tween of sprite `idx` of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): makes it visible on layer 0x10 at its saved z (`spriteZ[idx]`), clears its
   colour-add, resets the scale for `out` = 1, then `UiTweenBegin` (start 0, mode 3). */

void UiEquipStartSpriteFadeTween(UiEquip *self, u8 out, u16 idx)
{
  GfxSprite *sprite;

  /* The sprite table pointer is re-read from self->base.data before every access (as in the asm). */
  ((GfxSprite **)self->base.data)[idx]->flags |= 1;
  ((GfxSprite **)self->base.data)[idx]->layerMask = 0x10;
  ((GfxSprite **)self->base.data)[idx]->posZ = self->spriteZ[idx];
  sprite = ((GfxSprite **)self->base.data)[idx];
  sprite->addColor[0] = 0.0f;
  sprite->addColor[1] = 0.0f;
  sprite->addColor[2] = 0.0f;
  sprite->addColor[3] = 1.0f;
  sprite = ((GfxSprite **)self->base.data)[idx];
  if (out == 1) {
    UiSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f);
    sprite = ((GfxSprite **)self->base.data)[idx];
  }
  UiTweenBegin(0.0f, out, sprite, &self->tweens[idx], 3);
}
