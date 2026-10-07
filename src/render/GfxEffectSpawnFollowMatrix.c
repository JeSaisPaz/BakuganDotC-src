// bdc 0x0882429c GfxEffectSpawnFollowMatrix
#include "bdc.h"

/* Like `GfxEffectSpawnAtMatrix` but the effect keeps following `mtx`: it also stores the matrix
   pointer in `+0x164` and sets the local offset `+0x180` to the VFPU bank constant
   C730 = (0, 0, 0, 1), so `GfxEffectUpdateWorldPos` re-derives the position each frame. Returns
   the effect (`GfxSpriteLayerAdd`'s result). Used by `GameGimmickCameraCtor` and
   `ActorPlayerStateHoldRFocusPoint`. */

GfxEffect *GfxEffectSpawnFollowMatrix(GfxEffectMgr *mgr, s32 id, float *mtx)
{
  bool fromLow;
  GfxEffect *raw;
  GfxEffect *obj;
  GfxEffect *effect;
  float m[16];
  s32 i;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  raw = MemAlloc(sizeof(GfxEffect), (char *)0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  obj = (GfxEffect *)0;
  if (raw != (GfxEffect *)0) {
    GfxEffectCtor(raw, mgr, *(void **)(mgr->defs + id * 0x20), id);
    obj = raw;
  }
  effect = (GfxEffect *)GfxSpriteLayerAdd(&mgr->base, &obj->base);
  for (i = 0; i < 4; i++) {
    effect->pos[i] = mtx[12 + i];
  }
  /* All four rows are loaded before any is stored. */
  for (i = 0; i < 16; i++) {
    m[i] = mtx[i];
  }
  for (i = 0; i < 16; i++) {
    effect->matrix[i] = m[i];
  }
  effect->attachMatrix = mtx;
  effect->offset[0] = 0.0f;
  effect->offset[1] = 0.0f;
  effect->offset[2] = 0.0f;
  effect->offset[3] = 1.0f;
  return effect;
}
