// bdc 0x0882479c GfxEffectSetStateOwned
#include "bdc.h"

/* Writes `state` to `+0x170` of all effects of the manager `mgr` (list `+0x1c`) whose definition id
   `+0x16c` equals `id` (any when -1) and whose owner `+0x1fc` equals `owner` (any when NULL).
   Owner-based counterpart of `GfxEffectSetStateAttached`. Used by `ActorStopVexosBarrier`. */

void GfxEffectSetStateOwned(GfxEffectMgr *mgr, s32 id, void *owner, s32 state)

{
  GfxEffect *effect = (GfxEffect *)mgr->base.head;
  GfxEffect *next;

  while (effect != NULL) {
    next = (GfxEffect *)effect->base.next;
    if ((id == -1) || (effect->id == id)) {
      if (owner == NULL) {
        effect->key = state;
      } else if ((void *)effect->ownerBakugan == owner) {
        effect->key = state;
      }
    }
    effect = next;
  }
}
