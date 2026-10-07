// bdc 0x088a2198 ActorStageObjLandmarkBreak
#include "bdc.h"

/* Break virtual of the landmark (vtable `0x08af2434` slot 11, called by `ActorStageObjUpdate`
   when the object is destroyed): unless task 0x14a runs, stops its effects
   (`ActorStageObjStopOwnedEffects`), and in battle builds the 0x1e0-byte debris model
   `f6_landmark01_break.gmo` (`ActorStageObjDebrisCtor`) at the model position, plays sound
   0x20025c, launches the fragments (`ActorStageObjDebrisLaunch`), links the debris into the camera task's list
   (`+0x468`) for 60 frames with a cyan emissive colour and blended `kara`/`___` materials; then
   hides the landmark (alpha 0) and enters state 1 (`ActorStageObjLandmarkSetState`). */

void ActorStageObjLandmarkBreak(ActorStageObjLandmark *self)
{
  ActorStageObjDebris *debris;
  void *mem;
  bool fromLow;
  float *ty;
  float *row;
  float *bounds;
  float y;
  float d;
  GfxMaterialState *mat;
  BtlMain *camTask;
  float colour[4];
  float tmp[4];

  if (CoreTaskExists(0x14a) != 0) {
    return;
  }
  ActorStageObjStopOwnedEffects(&self->base);
  if (BtlCameraTaskExists() != 0) {
    debris = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(ActorStageObjDebris), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      ActorStageObjDebrisCtor(mem, "f6_landmark01_break.gmo");
      debris = (ActorStageObjDebris *)mem;
    }
    if (SndHasListener()) {
      SndEmitterCreateAtPos(SndGetListener(), 0x20025c, &self->base.base.data->rootMatrix[12], 0, 1);
    }
    /* raise the model's translation Y by the scaled bounds' min Y */
    ty = &self->base.base.data->rootMatrix[13];
    y = *ty;
    bounds = ActorStageObjGetBounds(&self->base);
    *ty = y - -(bounds[1] * self->base.base.scale[1]);
    y = self->base.base.data->rootMatrix[13];
    ActorStageObjDebrisLaunch(y, y, debris, self->base.base.data->rootMatrix, self->knockDir);
    /* matrix row 1 xyz += -minY * scaleY (w kept) */
    row = &self->base.base.data->rootMatrix[4];
    bounds = ActorStageObjGetBounds(&self->base);
    d = -(bounds[1] * self->base.base.scale[1]);
    tmp[0] = d;
    tmp[1] = d;
    tmp[2] = d;
    tmp[3] = 0.0f;
    row[0] = row[0] + tmp[0];
    row[1] = row[1] + tmp[1];
    row[2] = row[2] + tmp[2];
    camTask = (BtlMain *)BtlGetCameraTask();
    CoreObjectListAppend(&debris->base.base, &camTask->modelLists[1]);
    debris->lifetime = 60;
    debris->base.ambient[3] = 0.9f;
    debris->base.lighting = 1;
    colour[0] = 0.0f;
    colour[1] = 0.6f;
    colour[2] = 0.8f;
    colour[3] = 1.0f;
    GfxModelSetAmbientColor(&debris->base, colour, NULL);
    mat = GfxModelFindMaterialStateBySubstr(&debris->base, "f6_landmark01_kara_");
    if (mat != NULL) {
      mat->renderFlags = (mat->renderFlags & 0x3f) | 0x80;
      mat->shadeFlags = (mat->shadeFlags & 0x1f) | 0xa0;
      mat->renderFlags = (mat->renderFlags & 0xfc) | 0x02;
    }
    mat = GfxModelFindMaterialStateBySubstr(&debris->base, "f6_landmark01___");
    if (mat != NULL) {
      mat->renderFlags = (mat->renderFlags & 0x3f) | 0x40;
      mat->shadeFlags = (mat->shadeFlags & 0x1f) | 0xa0;
      mat->renderFlags = (mat->renderFlags & 0xfc) | 0x02;
    }
  }
  y = self->base.baseAlpha;
  self->base.fade = 0.0f;
  self->base.base.ambient[3] = y * 0.0f;
  ActorStageObjLandmarkSetState(self, 1);
}
