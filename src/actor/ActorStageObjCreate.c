// bdc 0x088a8cb8 ActorStageObjCreate
#include "bdc.h"

/* Creates one stage object (a scenery/prop actor such as `f0_building01.gmo`; the script opcodes
   call it a "unit"): allocates 0x340 bytes from the low heap, constructs it with
   `ActorStageObjCtor``(obj, kind, pos, instanceId)`, creates its HP gauge
   (`ActorStageObjEnsureHpGauge`), snaps the object's origin (`obj+0x20`) to the ground with
   `CollisionFindGroundPoint` (layer mask `0x3fbf2500`) and spawns effect `0x12d` at that point
   with `GfxEffectSpawn` on the effect manager `g_btlUnitEffectMgr`. Returns the new object
   (NULL if the allocation failed). */

void *ActorStageObjCreate(int kind, const float *pos, u32 instanceId)
{
  float ground[4] __attribute__((aligned(16)));
  bool fromLow;
  ActorStageObj *mem;
  ActorStageObj *obj;
  GfxEffect *effect;

  obj = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(ActorStageObj), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    ActorStageObjCtor(mem, kind, pos, instanceId);
    obj = mem;
  }
  ActorStageObjEnsureHpGauge(&obj->base);
  CollisionFindGroundPoint(ground, obj->base.base.pos, 0x3fbf2500);
  effect = GfxEffectSpawn(g_btlUnitEffectMgr, 0x12d, ground);
  effect->ownerBakugan = obj;
  if (obj != NULL) {
    effect->ownerId = obj->base.base.base.id;
  }
  return obj;
}
