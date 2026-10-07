// bdc 0x08823d34 GfxEffectMgrUpdate
#include "bdc.h"

/* Updates all effects of an effect manager (`GfxEffectMgrCtor`): advances the global update tick
   `g_gfxEffectTick`, clears `g_gfxEffectModelUpdatedMask`, snapshots the camera (`GfxEffectCaptureCamera`) and
   calls every effect's virtual update (slot 2, `GfxEffectUpdate`) along the list `+0x1c`. */

void GfxEffectMgrUpdate(GfxEffectMgr *mgr)
{
  GfxEffect *effect = (GfxEffect *)mgr->base.head;

  g_gfxEffectModelUpdatedMask = 0;
  g_gfxEffectTick = g_gfxEffectTick + 1;
  GfxEffectCaptureCamera();
  while (effect != NULL) {
    GfxEffect *next = (GfxEffect *)effect->base.next;
    const VtblEntry *update = &((const VtblEntry *)effect->base.vtable)[2];

    ((void (*)(void *))update->fn)((u8 *)effect + update->delta);
    effect = next;
  }
}
