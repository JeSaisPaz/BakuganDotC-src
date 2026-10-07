// bdc 0x089f48f4 UiSpriteSetSize
#include "bdc.h"

/* Stores width/height in the sprite and applies them with `GfxSpriteSetSize`. Note the argument
   order in the compiled code: the sprite is the third parameter. */

void UiSpriteSetSize(float w, float h, GfxSprite *sprite)

{
  sprite->width = w;
  sprite->height = h;
  GfxSpriteSetSize(sprite,w,h);
  return;
}

