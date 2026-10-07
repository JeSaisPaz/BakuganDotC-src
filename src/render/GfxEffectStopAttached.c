// bdc 0x08824618 GfxEffectStopAttached
#include "bdc.h"

/* Stops every effect on the manager `mgr` whose definition id (`+0x16c`) equals `id` (or any when
   `id == -1`) and whose attach pointer (`+0x160`) equals `attach` (any when `attach == NULL`) by
   handing it to `UiSpriteLayerRelease`; returns 1 if at least one effect matched, else 0. */

bool GfxEffectStopAttached(GfxEffectMgr *mgr, int id, void *attach)

{
  GfxEffect *effect;
  GfxEffect *next;
  bool found;

  found = false;
  next = (GfxEffect *)(mgr->base).head;
  while (effect = next, effect != (GfxEffect *)0x0) {
    next = (GfxEffect *)(effect->base).next;
    if (((id == -1) || (effect->id == id)) &&
        ((attach == (void *)0x0 || (effect->attachPos == (float *)attach)))) {
      found = true;
      UiSpriteLayerRelease(effect->mgr, effect);
    }
  }
  return found;
}

