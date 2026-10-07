// bdc 0x089f4924 GfxSpriteSetScaleRotation
#include "bdc.h"

/* Sets a sprite's model matrix (`+0x20`, 4x4 float, columns of 4) from a scale relative to its base
   size and a Z rotation: matrix = rotZ(angle) * diag(width*scaleX, height*scaleY, 0, 1), i.e.
   columns (sx*cos, sx*sin, 0, 0), (-sy*sin, sy*cos, 0, 0), (0, 0, 0, 0), (0, 0, 0, 1). When
   `angle == 0` and `force` is false it takes the cheap path
   `GfxSpriteSetSize``(width*scaleX, height*scaleY)` instead. Always sets flag 0x20 in `+0xd0`. */

void GfxSpriteSetScaleRotation(GfxSprite *sprite, float scaleX, float scaleY, float angle, bool force)

{
  float *m;
  float sx;
  float sy;
  float c;
  float s;
  float t;
  float q;

  sprite->flags = sprite->flags | 0x20;
  if (angle == 0.0f && !force) {
    GfxSpriteSetSize(sprite, sprite->width * scaleX, sprite->height * scaleY);
    return;
  }
  m = sprite->matrix;
  sx = sprite->width * scaleX;
  sy = sprite->height * scaleY;
  /* vrot takes quarter turns: angle * (2/pi). The VFPU reduces the quarter turns modulo 4 exactly
     before its sin/cos, so reduce here too: VfCosQuarter of a large t would round t * pi/2 first.
     t - 4*trunc(t/4) is exact; from |t/4| >= 2^23 on every float t is a multiple of 4. */
  t = angle * 0.636619772f;
  q = t * 0.25f;
  if (__builtin_fabsf(q) < 8388608.0f) {
    t = t - 4.0f * (float)(int)q;
  } else if (q - q == 0.0f) {
    t = 0.0f;
  }
  c = VfCosQuarter(t);
  s = VfSinQuarter(t);
  m[0] = sx * c;
  m[1] = sx * s;
  m[2] = 0.0f;
  m[3] = 0.0f;
  m[4] = sy * -s;
  m[5] = sy * c;
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
