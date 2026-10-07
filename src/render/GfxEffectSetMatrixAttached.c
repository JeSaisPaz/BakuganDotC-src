// bdc 0x08824a0c GfxEffectSetMatrixAttached
#include "bdc.h"

/* Copies the 4×4 matrix `mtx` to the transform `+0x20` of all effects of the manager `mgr` (list
   `+0x1c`) whose definition id `+0x16c` equals `id` (any when -1) and whose attach pointer `+0x160`
   equals `attach` (any when NULL). Used by `BtlAttackType6EUpdate`. */

void GfxEffectSetMatrixAttached(GfxEffectMgr *mgr, s32 id, const float *mtx, float *attach)
{
  GfxEffect *fx;
  GfxEffect *nextFx;

  for (fx = (GfxEffect *)mgr->base.head; fx != NULL; fx = nextFx) {
    nextFx = (GfxEffect *)fx->base.next;
    if ((id == -1 || fx->id == id) && (attach == NULL || fx->attachPos == attach)) {
      s32 i;

      for (i = 0; i < 16; i++) {
        fx->matrix[i] = mtx[i];
      }
    }
  }
}
