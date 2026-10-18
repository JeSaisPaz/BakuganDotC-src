// bdc 0x089f49f0 GfxSpriteResetMatrix
#include "bdc.h"

/* Rebuilds the sprite's model matrix (`+0x20..+0x5f`) as a pure scale by the base size: the
   VFPU path of `GfxSpriteSetScaleRotation` with scale 1 and angle 0, i.e.
   diag(width, height, 0, 1) (the rotation by 0 is the identity: cos 1, sin 0). */

void GfxSpriteResetMatrix(GfxSprite *sprite)
{
  float *m = sprite->matrix;

  m[0] = VfDot2(sprite->width, 0.0f, 1.0f, 0.0f);
  m[1] = VfDot2(sprite->width, 0.0f, 0.0f, 0.0f);
  m[2] = VfDot2(sprite->width, 0.0f, 0.0f, 0.0f);
  m[3] = VfDot2(sprite->width, 0.0f, 0.0f, 0.0f);
  m[4] = VfDot2(sprite->height, 0.0f, -0.0f, 0.0f);
  m[5] = VfDot2(sprite->height, 0.0f, 1.0f, 0.0f);
  m[6] = VfDot2(sprite->height, 0.0f, 0.0f, 0.0f);
  m[7] = VfDot2(sprite->height, 0.0f, 0.0f, 0.0f);
  m[8] = 0.0f;
  m[9] = 0.0f;
  m[10] = 0.0f;
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
}
