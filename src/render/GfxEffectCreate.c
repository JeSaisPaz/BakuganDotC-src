// bdc 0x08823e6c GfxEffectCreate
#include "bdc.h"

/* Allocates a 0x220-byte effect of definition `id` from the low heap, constructs it with
   `GfxEffectCtor` (definition `*(mgr[+0x88] + id*0x20)`) and appends it to the manager's object
   list (`GfxSpriteLayerAdd`). Unlike `GfxEffectSpawn` it sets no position; the caller fills `+0x60..`
   itself. Returns the effect (`GfxEffectEmitChildren`
   writes through it). */

void *GfxEffectCreate(GfxEffectMgr *mgr, int id)

{
  bool fromLow;
  GfxEffect *effect;
  GfxEffect *obj;

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
  return GfxSpriteLayerAdd(&mgr->base,&obj->base);
}
