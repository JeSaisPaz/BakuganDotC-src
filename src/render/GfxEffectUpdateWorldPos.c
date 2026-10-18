// bdc 0x0881df70 GfxEffectUpdateWorldPos
#include "bdc.h"

/* Recomputes an effect's world position from what it is attached to: when it follows a matrix
   (`+0x164`) the local offset `+0x180` (w forced to 1) is transformed by it (VFPU `vtfm4`) into
   `+0x60`, and, if flag `0x10` of `+0xd0` is set, the direction `+0x90` (w forced to 0) into
   `+0x80`; when it follows a position pointer (`+0x160`, see `GfxEffectSpawnAttached`) the
   position is `*attach + offset(+0x180)` in x, y, z, with w copied from `attach[3]`. */

static void GfxEffectTransform4(const float *m, const float *v, float *d)
{
  float r0 = VfDot4(m[0], m[4], m[8], m[12], v[0], v[1], v[2], v[3]);
  float r1 = VfDot4(m[1], m[5], m[9], m[13], v[0], v[1], v[2], v[3]);
  float r2 = VfDot4(m[2], m[6], m[10], m[14], v[0], v[1], v[2], v[3]);
  float r3 = VfDot4(m[3], m[7], m[11], m[15], v[0], v[1], v[2], v[3]);

  d[0] = r0;
  d[1] = r1;
  d[2] = r2;
  d[3] = r3;
}

void GfxEffectUpdateWorldPos(GfxEffect *effect)
{
  if (effect->attachMatrix != NULL) {
    effect->offset[3] = 1.0f;
    GfxEffectTransform4(effect->attachMatrix, effect->offset, effect->pos);
    if ((effect->flags & 0x10) != 0) {
      effect->dir[3] = 0.0f;
      GfxEffectTransform4(effect->attachMatrix, effect->dir, effect->worldDir);
    }
  }
  if (effect->attachPos != NULL) {
    float x, y, z, w;

    effect->offset[3] = 1.0f;
    x = VfAdd(effect->attachPos[0], effect->offset[0]);
    y = VfAdd(effect->attachPos[1], effect->offset[1]);
    z = VfAdd(effect->attachPos[2], effect->offset[2]);
    w = effect->attachPos[3];
    effect->pos[0] = x;
    effect->pos[1] = y;
    effect->pos[2] = z;
    effect->pos[3] = w;
  }
}
