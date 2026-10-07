// bdc 0x088a743c ActorStageObjAttrLandmarkDraw
#include "bdc.h"

/* Draw method of the attribute landmark (vtable `0x08af27b4` slot 8). Does nothing when the model
   alpha (ambient[3]) is <= 0. Otherwise draws the model with alpha scaled by `alpha`
   (`GfxModelDlWriteState`); without a companion it then draws a second hologram pass with alpha
   ramping by `scroll` (scroll * 2 below 0.5, (16 - scroll) / 8 above 8, else 1) and every material
   routed through `ActorStageObjAttrLandmarkWriteScanDl` (`GfxModelSetMaterialAnimCallbackByIndex`,
   cleared again afterwards), and finally sets alpha 0.9. */

void ActorStageObjAttrLandmarkDraw(ActorStageObjAttrLandmark *self, u32 **dl)
{
  float alpha;
  float scroll;
  s32 i;

  alpha = self->base.base.ambient[3];
  if (!(alpha <= 0.0f)) {
    self->base.base.ambient[3] = alpha * self->alpha;
    GfxModelDlWriteState(&self->base.base, dl);
    if (self->companion == NULL) {
      scroll = self->scroll;
      alpha = 1.0f;
      if (!(scroll <= 8.0f)) {
        alpha = (16.0f - scroll) * 0.125f;
      }
      if (scroll < 0.5f) {
        alpha = scroll * 2.0f;
      }
      self->base.base.ambient[3] = alpha;
      for (i = 0; i < self->base.base.materialCount; i++) {
        GfxModelSetMaterialAnimCallbackByIndex(&self->base.base, i,
                                               ActorStageObjAttrLandmarkWriteScanDl, &self->scroll);
      }
      GfxModelDlWriteState(&self->base.base, dl);
      for (i = 0; i < self->base.base.materialCount; i++) {
        GfxModelSetMaterialAnimCallbackByIndex(&self->base.base, i, NULL, NULL);
      }
    }
    self->base.base.ambient[3] = 0.9f;
  }
}
