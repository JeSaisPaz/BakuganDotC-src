// bdc 0x08824554 GfxEffectSpawnWithOwner
#include "bdc.h"

/* `GfxEffectSpawn` followed by setting the owner (`+0x1fc`) and, when the owner is non-NULL, the
   owner's id word (`owner+0xc` → `+0x200`) of the new effect. Returns the new effect. Used by
   `ActorStartVexosBarrier` and battle code. */

GfxEffect *GfxEffectSpawnWithOwner(GfxEffectMgr *mgr, s32 id, float *pos, void *owner)
{
  GfxEffect *effect = (GfxEffect *)GfxEffectSpawn(mgr, id, pos);

  effect->ownerBakugan = owner;
  if (owner != NULL) {
    effect->ownerId = ((CoreObject *)owner)->id;
  }
  return effect;
}
