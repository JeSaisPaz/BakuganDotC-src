// bdc 0x08824398 GfxEffectSpawnAttached
#include "bdc.h"

/* Like `GfxEffectSpawn` but also remembers `attach` (a pointer to a 4-float position, not a copy)
   in `effect+0x160`: allocates a 0x220-byte effect on the manager `mgr`, constructs it with
   `GfxEffectCtor` for definition `id`, appends it to the manager list (`mgr+0x1c`), copies the
   vector at `attach` to `effect+0x60..0x6c`, stores the pointer itself in `effect+0x160` and sets
   `offset` (`+0x180`) to (0, 0, 0, 1).
   Returns the effect. */

void *GfxEffectSpawnAttached(GfxEffectMgr *mgr, int id, float *attach)

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
  e->pos[0] = attach[0];
  e->pos[1] = attach[1];
  e->pos[2] = attach[2];
  e->pos[3] = attach[3];
  e->attachPos = attach;
  /* C730 of the VFPU constant bank: (0, 0, 0, 1). */
  e->offset[0] = 0.0f;
  e->offset[1] = 0.0f;
  e->offset[2] = 0.0f;
  e->offset[3] = 1.0f;
  return e;
}
