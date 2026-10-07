// bdc 0x089505a0 UiTitleMenuBuildSprites
#include "bdc.h"

/* Creates the 5 layout sprites of `UiTitleMenu` (`UiLayoutCreateSprites`) and
   resets sprite 0 to centred pivot, linear filtering, scale 1, angle 0. */

void UiTitleMenuBuildSprites(UiScreen *screen)

{
  GfxSprite *sprite;
  
  UiLayoutCreateSprites(screen->spriteLayer,screen->data,5);
  sprite = *(GfxSprite **)screen->data;
  GfxSpriteCenterPivot(sprite);
  sprite->flags = sprite->flags | 0x20;
  sprite->scaleY = 1.0;
  sprite->scaleX = 1.0;
  sprite->angle = 0.0;
  GfxSpriteSetScaleRotation(sprite,1.0,1.0,0.0,false);
  return;
}

