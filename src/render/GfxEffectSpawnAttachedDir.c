// bdc 0x0882446c GfxEffectSpawnAttachedDir
#include "bdc.h"

/* Spawns `effect` `id` on `mgr` attached to the position pointer `attach`
   (`+0x160`) with direction/velocity `dir` (vec4 → `+0x90`); position `+0x60` and local offset
   `+0x180` start at the VFPU bank constant C730 = (0, 0, 0, 1). Returns the effect
   (`GfxSpriteLayerAdd`'s result). Used by Bakugan hit/block effects
   (`BtlBakuganOnHit`, `BtlBakuganTryBlock`, …). */

void *GfxEffectSpawnAttachedDir(GfxEffectMgr *mgr, s32 id, float *attach, const float *dir)
{
  bool fromLow;
  GfxEffect *raw;
  GfxEffect *obj;
  GfxEffect *e;

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
  e = (GfxEffect *)GfxSpriteLayerAdd(&mgr->base, &obj->base);
  e->pos[0] = 0.0f;
  e->pos[1] = 0.0f;
  e->pos[2] = 0.0f;
  e->pos[3] = 1.0f;
  e->attachPos = attach;
  e->dir[0] = dir[0];
  e->dir[1] = dir[1];
  e->dir[2] = dir[2];
  e->dir[3] = dir[3];
  e->offset[0] = 0.0f;
  e->offset[1] = 0.0f;
  e->offset[2] = 0.0f;
  e->offset[3] = 1.0f;
  return e;
}
