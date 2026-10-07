// bdc 0x088ac540 ActorStageObjUpdateLightPositions
#include "bdc.h"

/* Moves each light billboard of the object (`+0x164[i]`) to its model-space position (`light+0x90`)
   transformed by the model matrix (`MathMtx4TransformPoint`). Called by `ActorStageObjUpdate` after state
   changes. */

void ActorStageObjUpdateLightPositions(ActorStageObjBase *self)
{
  ScePspFVector4 pos;
  int i;

  if (self->lights != (GfxSprite **)0x0) {
    for (i = 0; i < self->lightCount; i++) {
      GfxSprite *light = self->lights[i];

      if (light != (GfxSprite *)0x0) {
        MathMtx4TransformPoint((const ScePspFMatrix4 *)self->base.data->rootMatrix, &pos,
                               (const ScePspFVector4 *)&light->scaleX);
        /* lv.q/sv.q: the whole vec4 (w included) into posX..posW. */
        light->posX = pos.x;
        light->posY = pos.y;
        light->posZ = pos.z;
        light->posW = pos.w;
      }
    }
  }
}
