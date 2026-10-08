// bdc 0x088daa44 GameGimmickEffectMarkerDisable
#include "bdc.h"

/* Vtable `0x08af3524` slot 16: when active (`+0x15e`), clears the flag, releases the two looping
   effects `+0x180`/`+0x184` from their layer (`UiSpriteLayerRelease` on `effect + 0x214`) and
   destroys the attached object `+0x174`. */

void GameGimmickEffectMarkerDisable(GameGimmickEffectMarker *gimmick)
{
  void **slot;
  int i;

  if (gimmick->base.active != 0) {
    gimmick->base.active = 0;
    slot = &gimmick->effect;
    for (i = 0; i < 2; i++) {
      GfxEffect *effect = (GfxEffect *)slot[i];

      if (effect != NULL) {
        UiSpriteLayerRelease(effect->mgr, effect);
      }
    }
    if (gimmick->base.attached == NULL) {
      gimmick->base.attached = NULL;
    } else {
      CoreNode *obj = (CoreNode *)gimmick->base.attached;
      const VtblEntry *entry = &((const VtblEntry *)obj->vtable)[1];

      ((void (*)(void *, int))entry->fn)((u8 *)obj + entry->delta, 3);
      gimmick->base.attached = NULL;
      gimmick->base.attached = NULL;
    }
  }
}
