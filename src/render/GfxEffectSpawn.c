// bdc 0x08823f1c GfxEffectSpawn
#include "bdc.h"

/* Creates one effect (particle/animation object) of definition `id` on the effect manager `mgr`
   (global `g_btlUnitEffectMgr`): allocates 0x220 bytes from the low heap, constructs it with
   `GfxEffectCtor(effect, mgr, *(mgr[+0x88] + id * 0x20), id)`, appends it to the manager's object
   list (`GfxSpriteLayerAdd(mgr, effect)`, list head at `mgr+0x1c`) and copies the 4-float position `pos`
   to `effect+0x60..0x6c`. Returns the effect (callers such as
   `ActorStageObjCreate` use it). */

void *GfxEffectSpawn(GfxEffectMgr *mgr, int id, const float *pos)

{
  bool fromLow;
  GfxEffect *effect;
  GfxEffect *obj;
  GfxEffect *e;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  effect = MemAlloc(0x220,(char *)0x0,0);
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
  return e;
}
