// bdc 0x08970728 UiOptionShowSprite
#include "bdc.h"

/* Makes a sprite of `UiOption` visible with its pivot centred
   (`GfxSpriteCenterPivot`), linear filtering and scale 1. The screen argument is unused. */

void UiOptionShowSprite(UiScreen *screen, GfxSprite *sprite)

{
  sprite->flags = sprite->flags | 1;
  GfxSpriteCenterPivot(sprite);
  sprite->flags = sprite->flags | 0x20;
  UiSpriteSetScaleRotation(sprite,1.0,1.0,0.0);
  return;
}

