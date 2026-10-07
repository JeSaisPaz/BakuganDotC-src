// bdc 0x089a51bc UiSpriteSetScaleRotation
#include "bdc.h"

/* Stores a 2D UI sprite's scale (X/Y) and rotation angle, then rebuilds the sprite's local
   transform matrix. */

void UiSpriteSetScaleRotation(GfxSprite *sprite, float scaleX, float scaleY, float angle)

{
  sprite->scaleX = scaleX;
  sprite->scaleY = scaleY;
  sprite->angle = angle;
  GfxSpriteSetScaleRotation(sprite,sprite->scaleX,sprite->scaleY,angle,false);
  return;
}

