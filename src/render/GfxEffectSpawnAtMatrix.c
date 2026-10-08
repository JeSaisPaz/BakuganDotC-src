// bdc 0x088241ac GfxEffectSpawnAtMatrix
#include "bdc.h"

/* Spawns `effect` `id` on `mgr` and initialises its transform from the 4×4 matrix
   `mtx`: its translation row (`mtx[12..15]`) is copied to the position `+0x60`, then the whole
   matrix to `+0x20`. Returns the effect: the value `GfxSpriteLayerAdd` returns (the appended
   object). Used by actor/NPC code (`ActorNpcSwitchRobotStateShutdown`, …). */

GfxEffect *GfxEffectSpawnAtMatrix(GfxEffectMgr *mgr, s32 id, const float *mtx)
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
    GfxEffectCtor(raw, mgr, PspPtr(*(u32 *)(mgr->defs + id * 0x20)), id);
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
  return effect;
}
