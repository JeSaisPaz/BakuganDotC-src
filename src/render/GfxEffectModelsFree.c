// bdc 0x088252dc GfxEffectModelsFree
#include "bdc.h"

/* Deletes the shared effect models of `GfxEffectModelsLoad` (virtual destructor, flags 3) and
   clears their slots. Called by `BtlMainTaskDtor`, `BtlMainTeardown` and `GameFieldDtor`. */

void GfxEffectModelsFree(void)

{
  GfxModel **slot;
  int i;

  i = 0;
  if (0 < g_gfxEffectModelCount) {
    slot = g_gfxEffectModels;
    do {
      GfxModel *model = *slot;
      if (model != (GfxModel *)0x0) {
        const VtblEntry *vt = (const VtblEntry *)model->base.vtable;
        ((void (*)(void *, int))vt[1].fn)((char *)model + vt[1].delta, 3);
        *slot = (GfxModel *)0x0;
      }
      i = i + 1;
      slot = slot + 1;
    } while (i < g_gfxEffectModelCount);
  }
}
