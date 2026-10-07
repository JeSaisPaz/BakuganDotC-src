// bdc 0x08823db8 GfxEffectMgrUpdateOwner
#include "bdc.h"

/* Like `GfxEffectMgrUpdate` but only updates effects whose owner (`+0x1fc`) is `g_btlArenaModels[0]`
   (the current focus Bakugan), e.g. while the rest of the scene is frozen during a special-attack
   cut-in. Called by `BtlMainUpdateStageEffects`. */

void GfxEffectMgrUpdateOwner(GfxEffectMgr *mgr)
{
  GfxEffect *effect = (GfxEffect *)mgr->base.head;

  g_gfxEffectModelUpdatedMask = 0;
  g_gfxEffectTick = g_gfxEffectTick + 1;
  GfxEffectCaptureCamera();
  while (effect != NULL) {
    GfxEffect *next = (GfxEffect *)effect->base.next;

    if (effect->ownerBakugan == (void *)g_btlArenaModels[0]) {
      const VtblEntry *update = &((const VtblEntry *)effect->base.vtable)[2];

      ((void (*)(void *))update->fn)((u8 *)effect + update->delta);
    }
    effect = next;
  }
}
