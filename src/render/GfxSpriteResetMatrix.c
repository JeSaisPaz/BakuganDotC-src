// bdc 0x089f49f0 GfxSpriteResetMatrix
#include "bdc.h"

/* Rebuilds the sprite's model matrix (`+0x20..+0x5f`) as a pure scale by the base size: the
   VFPU path of `GfxSpriteSetScaleRotation` with scale 1 and angle 0, i.e.
   diag(width, height, 0, 1) (the rotation by 0 is the identity: cos 1, sin 0). */

void GfxSpriteResetMatrix(GfxSprite *sprite)
{
  float *m = sprite->matrix;

  m[0] = sprite->width;
  m[1] = 0.0f;
  m[2] = 0.0f;
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = sprite->height;
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = 0.0f;
  m[9] = 0.0f;
  m[10] = 0.0f;
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
}
