// bdc 0x08824968 GfxEffectFindAttached
#include "bdc.h"

/* Returns the first of the effects of the manager `mgr` (list `+0x1c`) whose definition id `+0x16c`
   equals `id` (any when -1) and whose attach pointer `+0x160` equals `attach` (any when NULL), or
   NULL. */

void * GfxEffectFindAttached(GfxEffectMgr *mgr, s32 id, float *attach)
{
  GfxEffect *e;

  for (e = (GfxEffect *)mgr->base.head; e != NULL; e = (GfxEffect *)e->base.next) {
    if ((id == -1 || e->id == id) && (attach == NULL || e->attachPos == attach)) {
      return e;
    }
  }
  return NULL;
}
