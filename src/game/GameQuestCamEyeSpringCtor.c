// bdc 0x088f8880 GameQuestCamEyeSpringCtor
#include "bdc.h"

/* Constructor of the eye spring of the quest-field camera system (see `GameQuestCamCtrlCtor`)
   (vtable `g_gameQuestCamEyeSpringVtbl`): spring base (`GameQuestCamSpringCtor`), look-at
   `desc+0x30` -> `+0x60`, default vector (`zero` of `g_gameQuestCamEyeSpringAxisConsts`) ->
   `+0x70`, path set `desc+0x40` -> `+0x90`, and the initial eye-to-target distance
   `|ctrl->eye - ctrl->lookAt|` (xyz) -> `+0x94`. Returns `self`. */

typedef struct CamEyeSpringDesc {
  GameQuestCamSpringDesc base;
  ScePspFVector4 point;
  void *pathSet;
} CamEyeSpringDesc;

GameQuestCamEyeSpring *GameQuestCamEyeSpringCtor(GameQuestCamEyeSpring *self, void *desc)
{
  CamEyeSpringDesc *d = (CamEyeSpringDesc *)desc;
  GameQuestCamCtrl *ctrl;
  float dx;
  float dy;
  float dz;

  GameQuestCamSpringCtor(&self->base, desc);
  self->base.vtbl = g_gameQuestCamSpringBaseVtbl;
  self->point = d->point;
  self->axis = g_gameQuestCamEyeSpringAxisConsts.zero;
  self->base.vtbl = g_gameQuestCamEyeSpringVtbl;
  self->pathSet = d->pathSet;
  ctrl = self->base.ctrl;
  dx = ctrl->eye.x - ctrl->lookAt.x;
  dy = ctrl->eye.y - ctrl->lookAt.y;
  dz = ctrl->eye.z - ctrl->lookAt.z;
  self->dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
  return self;
}
