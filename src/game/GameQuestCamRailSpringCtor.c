// bdc 0x088f904c GameQuestCamRailSpringCtor
#include "bdc.h"

/* Constructor of a third quest camera spring (vtable `g_gameQuestCamRailSpringVtbl`, built by
   `GameQuestCamLookFactoryBuild`): spring base, look-at `desc+0x30`, default vector (`zero` of
   `g_gameQuestCamRailSpringAxisConsts`), path set `desc+0x40` -> `+0x90` and the initial
   distance `+0x94` = |eye - lookAt| of the owning camera control. */

typedef struct CamRailSpringDesc {
  GameQuestCamSpringDesc base;
  ScePspFVector4 point;
  void *pathSet;
} CamRailSpringDesc;

void *GameQuestCamRailSpringCtor(void *spring, void *desc)
{
  GameQuestCamEyeSpring *self = (GameQuestCamEyeSpring *)spring;
  CamRailSpringDesc *d = (CamRailSpringDesc *)desc;
  GameQuestCamCtrl *ctrl;
  float dx, dy, dz;

  GameQuestCamSpringCtor(&self->base, desc);
  self->base.vtbl = g_gameQuestCamSpringBaseVtbl;
  self->point = d->point;
  self->axis = g_gameQuestCamRailSpringAxisConsts.zero;
  self->base.vtbl = g_gameQuestCamRailSpringVtbl;
  self->pathSet = d->pathSet;
  ctrl = self->base.ctrl;
  dx = ctrl->eye.x - ctrl->lookAt.x;
  dy = ctrl->eye.y - ctrl->lookAt.y;
  dz = ctrl->eye.z - ctrl->lookAt.z;
  self->dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
  return self;
}
