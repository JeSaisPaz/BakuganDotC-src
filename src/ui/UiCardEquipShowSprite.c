// bdc 0x0896a254 UiCardEquipShowSprite
#include "bdc.h"

/* Makes a sprite of `UiCardEquip` visible with its pivot centred
   (`GfxSpriteCenterPivot`), linear filtering (flag 0x20) and scale 1, rotation 0. The screen
   argument is unused. */

void UiCardEquipShowSprite(UiCardEquip *self, GfxSprite *sprite)
{
  sprite->flags = sprite->flags | 1;
  GfxSpriteCenterPivot(sprite);
  sprite->flags = sprite->flags | 0x20;
  UiSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f);
}
