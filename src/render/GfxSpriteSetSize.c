// bdc 0x089f4504 GfxSpriteSetSize
#include "bdc.h"

/* Sets a sprite's on-screen size by writing the scale diagonal of its model matrix (`+0x20` = x
   scale, `+0x34` = y scale, `+0x48` = 1.0). For top-left-pivot quad modes (even `+0xe4` other than
   4) the values are doubled, because those templates span 0..0.5; centred modes (odd) and mode 4
   use them as-is. */

void GfxSpriteSetSize(GfxSprite *sprite, float width, float height)

{
  if (sprite->quadMode == 4) {
    sprite->matrix[0] = width;
    sprite->matrix[5] = height;
    sprite->matrix[10] = 1.0f;
    return;
  }
  if ((sprite->quadMode & 1U) != 0) {
    sprite->matrix[0] = width;
    sprite->matrix[5] = height;
    sprite->matrix[10] = 1.0f;
    return;
  }
  sprite->matrix[10] = 1.0f;
  sprite->matrix[0] = width * 2.0f;
  sprite->matrix[5] = height * 2.0f;
  return;
}

