// bdc 0x08823fe4 GfxEffectSpawnDirected
#include "bdc.h"

/* Like `GfxEffectSpawn` but also sets a direction: allocates a 0x220-byte effect from the low
   heap, constructs it from definition `id` of the effect manager `mgr` (`GfxEffectCtor`,
   definitions at `mgr+0x88`, stride 0x20), adds it to the manager's layer (`GfxSpriteLayerAdd`)
   and copies the 4-float position to `+0x60` and the direction/velocity to `+0x90`. Returns the
   effect (the object `GfxSpriteLayerAdd` passes back from `CoreObjectListAppend`; callers such
   as `BtlAttackUpdateTrailedHomingShot` write its matrix). */

GfxEffect *GfxEffectSpawnDirected(GfxEffectMgr *mgr, int id, float *pos, float *dir)

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
    GfxEffectCtor(effect,mgr,PspPtr(*(u32 *)(mgr->defs + id * 0x20)),id);
    obj = effect;
  }
  e = (GfxEffect *)GfxSpriteLayerAdd(&mgr->base,&obj->base);
  e->pos[0] = pos[0];
  e->pos[1] = pos[1];
  e->pos[2] = pos[2];
  e->pos[3] = pos[3];
  e->dir[0] = dir[0];
  e->dir[1] = dir[1];
  e->dir[2] = dir[2];
  e->dir[3] = dir[3];
  return e;
}
