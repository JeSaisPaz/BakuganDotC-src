// bdc 0x0882515c GfxEffectModelUpdateOnce
#include "bdc.h"

/* Runs the per-frame virtual update (vtable slot 7, `+0x38`) of preloaded effect model `index`
   (table `0x08ab9e34`, see `GfxEffectModelsLoad`) at most once per update tick: the bit `1 << index` of `g_gfxEffectModelUpdatedMask` (cleared by `GfxEffectMgrUpdate`) remembers models
   already updated. Called from `BtlBakuganUpdateStatusVisuals`. */

void GfxEffectModelUpdateOnce(u32 index)
{
  u32 bit = 1 << (index & 0x1f);
  u32 mask = g_gfxEffectModelUpdatedMask;
  GfxModel *model;

  if ((mask & bit) == 0) {
    g_gfxEffectModelUpdatedMask = mask | bit;
    model = g_gfxEffectModels[index];
    if (model != NULL) {
      const VtblEntry *update = &((const VtblEntry *)model->base.vtable)[7];

      ((void (*)(void *))update->fn)((u8 *)model + update->delta);
    }
  }
}
