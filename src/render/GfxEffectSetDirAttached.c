// bdc 0x088249b8 GfxEffectSetDirAttached
#include "bdc.h"

/* Copies the vector `dir` to the direction/velocity `+0x90` of all effects of the manager `mgr`
   (list `+0x1c`) whose definition id `+0x16c` equals `id` (any when -1) and whose attach pointer
   `+0x160` equals `attach` (any when NULL). Used by `BtlAttackType6EUpdate`,
   `BtlAttackType7CUpdate`. */

void GfxEffectSetDirAttached(GfxEffectMgr *mgr, s32 id, const float *dir, float *attach)
{
  GfxEffect *fx;
  GfxEffect *nextFx;

  for (fx = (GfxEffect *)mgr->base.head; fx != NULL; fx = nextFx) {
    nextFx = (GfxEffect *)fx->base.next;
    if ((id == -1 || fx->id == id) && (attach == NULL || fx->attachPos == attach)) {
      fx->dir[0] = dir[0];
      fx->dir[1] = dir[1];
      fx->dir[2] = dir[2];
      fx->dir[3] = dir[3];
    }
  }
}
