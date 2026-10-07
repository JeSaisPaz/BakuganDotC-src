// bdc 0x088240c4 GfxEffectSpawnOwned
#include "bdc.h"

/* Spawns `effect` `id` on the effect manager (`GfxEffectMgrCtor`) `mgr` (0x220
   bytes, low heap, `GfxEffectCtor`, added with `GfxSpriteLayerAdd`) at position `pos` (vec4 →
   `+0x60`) and records its owner `owner` (`+0x1fc`; `BtlStageUpdateAmbientEffects` passes the
   first arena model) plus the owner's id word (`owner+0xc` → `+0x200`), and returns the effect
   (the `GfxSpriteLayerAdd` result left in `v0`). Used by `BtlStageUpdateAmbientEffects`. */

GfxEffect *GfxEffectSpawnOwned(GfxEffectMgr *mgr, s32 id, const float *pos, void *owner)

{
  bool fromLow;
  GfxEffect *effect;
  GfxEffect *obj;
  GfxEffect *e;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  effect = MemAlloc(sizeof(GfxEffect),(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  obj = (GfxEffect *)0x0;
  if (effect != (GfxEffect *)0x0) {
    GfxEffectCtor(effect,mgr,*(void **)(mgr->defs + id * 0x20),id);
    obj = effect;
  }
  e = (GfxEffect *)GfxSpriteLayerAdd(&mgr->base,&obj->base);
  e->pos[0] = pos[0];
  e->pos[1] = pos[1];
  e->pos[2] = pos[2];
  e->pos[3] = pos[3];
  e->ownerBakugan = owner;
  if (owner != (void *)0x0) {
    e->ownerId = ((CoreObject *)owner)->id;
  }
  return e;
}
