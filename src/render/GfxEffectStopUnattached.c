// bdc 0x08824588 GfxEffectStopUnattached
#include "bdc.h"

/* Stops (releases through `UiSpriteLayerRelease` on their manager) all effects of the manager
   `mgr` (list `+0x1c`) whose definition id `+0x16c` equals `id` (any when -1) that have neither an
   attach pointer (`+0x160`) nor an owner (`+0x1fc`). Returns 1 if any matched. Used by
   `BtlMainStartResultDemo`. */

s32 GfxEffectStopUnattached(GfxEffectMgr *mgr, s32 id)

{
  GfxEffect *effect;
  GfxEffect *next;
  s32 found;

  found = 0;
  next = (GfxEffect *)(mgr->base).head;
  while (effect = next, effect != (GfxEffect *)0x0) {
    next = (GfxEffect *)(effect->base).next;
    if (((id == -1) || (effect->id == id)) && (effect->attachPos == (float *)0x0) &&
        (effect->ownerBakugan == (void *)0x0)) {
      found = 1;
      UiSpriteLayerRelease(effect->mgr, effect);
    }
  }
  return found;
}

