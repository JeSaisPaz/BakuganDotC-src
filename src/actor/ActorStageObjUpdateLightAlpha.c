// bdc 0x088ac460 ActorStageObjUpdateLightAlpha
#include "bdc.h"

/* Sets the alpha of the object's light billboards (`lights[0..lightCount)`, added by
   `ActorStageObjAttachLight`) to the draw alpha (`ambient[3]`) x 0.85 (0.6 for kind 0x1b) while
   `g_btlHudHidden` is nonzero, else to 0. Does nothing without a light list. Called by
   `ActorStageObjUpdate`. */

void ActorStageObjUpdateLightAlpha(ActorStageObjBase *self)
{
  float scale;
  s32 i;

  if (self->lights == NULL) {
    return;
  }
  scale = 0.85f;
  if (self->kind == 0x1b) {
    scale = 0.6f;
  }
  if (g_btlHudHidden == 0) {
    for (i = 0; i < self->lightCount; i++) {
      if (self->lights[i] != NULL) {
        self->lights[i]->alpha = 0.0f;
      }
    }
  } else {
    for (i = 0; i < self->lightCount; i++) {
      if (self->lights[i] != NULL) {
        self->lights[i]->alpha = self->base.ambient[3] * scale;
      }
    }
  }
}
