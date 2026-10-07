// bdc 0x0895f010 UiEquipStartFourPlayerDecorTween
#include "bdc.h"

/* Starts the appear (`out` = 0) or disappear (`out` = 1) tween (`UiTweenBegin`, scale 1.5, mode
   3) of the three decoration sprites of the four-player layout (sprite range ``+0x5180`, three
   sprites`) on the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`) (four-player
   layout only; the first sprite is mirrored with `GfxSpriteFlipU`; tween mode 1 = alpha only). */

void UiEquipStartFourPlayerDecorTween(UiEquip *self, u8 out)

{
  GfxSprite **sprites;
  GfxSprite *sprite;
  u32 i;

  if (self->playerCount > 2) {
    for (i = self->spriteIdx[0x10]; (s32)i < (s32)(self->spriteIdx[0x10] + 3); i++) {
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
      if (i == self->spriteIdx[0x10]) {
        GfxSpriteFlipU(sprite);
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      UiTweenBegin(1.5f, out, sprite, &self->tweens[i], 1);
    }
  }
}
