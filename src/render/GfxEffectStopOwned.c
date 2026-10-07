// bdc 0x088246b4 GfxEffectStopOwned
#include "bdc.h"

/* Stops all effects of the manager `mgr` (list `+0x1c`) whose definition id `+0x16c` equals `id`
   (any when -1) and whose owner `+0x1fc` equals `owner` (any when NULL), releasing them with
   `UiSpriteLayerRelease`. Returns 1 if any matched. Owner-based counterpart of
   `GfxEffectStopAttached`. */

s32 GfxEffectStopOwned(GfxEffectMgr *mgr, s32 id, void *owner)

{
  GfxEffect *effect;
  GfxEffect *next;
  s32 found;

  found = 0;
  next = (GfxEffect *)(mgr->base).head;
  while (effect = next, effect != (GfxEffect *)0x0) {
    next = (GfxEffect *)(effect->base).next;
    if (((id == -1) || (effect->id == id)) &&
        ((owner == (void *)0x0 || (effect->ownerBakugan == owner)))) {
      found = 1;
      UiSpriteLayerRelease(effect->mgr, effect);
    }
  }
  return found;
}
