// bdc 0x088248d4 GfxEffectUpdateAttachedNow
#include "bdc.h"

/* Immediately runs the virtual update (slot 2, `GfxEffectUpdate`) of all effects of the manager
   `mgr` (list `+0x1c`) whose definition id `+0x16c` equals `id` (any when -1) and whose attach
   pointer `+0x160` equals `attach` (any when NULL). */

void GfxEffectUpdateAttachedNow(GfxEffectMgr *mgr, s32 id, float *attach)

{
  GfxEffect *effect = (GfxEffect *)mgr->base.head;

  while (effect != (GfxEffect *)0x0) {
    GfxEffect *next = (GfxEffect *)effect->base.next;
    if ((id == -1 || effect->id == id) && (attach == (float *)0x0 || effect->attachPos == attach)) {
      const VtblEntry *vt = (const VtblEntry *)effect->base.vtable;
      ((void (*)(void *))vt[2].fn)((char *)effect + vt[2].delta);
    }
    effect = next;
  }
}
