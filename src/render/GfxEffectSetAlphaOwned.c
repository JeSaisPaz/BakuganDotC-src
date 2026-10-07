// bdc 0x0882483c GfxEffectSetAlphaOwned
#include "bdc.h"

/* Sets the alpha `+0xbc` to `alpha` on all effects of the manager `mgr` (list `+0x1c`) whose
   definition id `+0x16c` equals `id` (any when -1) and whose owner `+0x1fc` equals `owner` (any
   when NULL). */

void GfxEffectSetAlphaOwned(float alpha, GfxEffectMgr *mgr, s32 id, void *owner)
{
  GfxEffect *e = (GfxEffect *)mgr->base.head;
  GfxEffect *next;

  for (; e != NULL; e = next) {
    next = (GfxEffect *)e->base.next;
    if (id == -1 || e->id == id) {
      if (owner == 0 || e->ownerBakugan == owner) {
        e->color[3] = alpha;
      }
    }
  }
}
