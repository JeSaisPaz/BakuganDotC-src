// bdc 0x08824888 GfxEffectSetAlphaAttached
#include "bdc.h"

/* Sets the alpha `+0xbc` to `alpha` on all effects of the manager `mgr` (list `+0x1c`) whose
   definition id `+0x16c` equals `id` (any when -1) and whose attach pointer `+0x160` equals
   `attach` (any when NULL). */

void GfxEffectSetAlphaAttached(float alpha, GfxEffectMgr *mgr, s32 id, float *attach)
{
  GfxEffect *e = (GfxEffect *)mgr->base.head;
  GfxEffect *next;

  for (; e != NULL; e = next) {
    next = (GfxEffect *)e->base.next;
    if (id == -1 || e->id == id) {
      if (attach == 0 || e->attachPos == attach) {
        e->color[3] = alpha;
      }
    }
  }
}
