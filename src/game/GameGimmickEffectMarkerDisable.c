// bdc 0x088daa44 GameGimmickEffectMarkerDisable
#include "bdc.h"

/* Vtable `0x08af3524` slot 16: when active (`+0x15e`), clears the flag, releases the two looping
   effects `+0x180`/`+0x184` from their layer (`UiSpriteLayerRelease` on `effect + 0x214`) and
   destroys the attached object `+0x174`. */

/* Attached collider as seen here: only its virtual destructor table at `+0x20` is used. */
typedef struct GameGimmickAttachedView {
  u8 _pad[0x20];
  const VtblEntry *vtbl;
} GameGimmickAttachedView;

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
      GameGimmickAttachedView *obj = (GameGimmickAttachedView *)gimmick->base.attached;
      const VtblEntry *entry = &obj->vtbl[1];

      ((void (*)(void *, int))entry->fn)((u8 *)obj + entry->delta, 3);
      gimmick->base.attached = NULL;
      gimmick->base.attached = NULL;
    }
  }
}
