// bdc 0x088f6994 GameQuestCamPathModeCtor
#include "bdc.h"

/* Constructor of the path-following (default) camera mode of the quest-field camera system (see
   `GameQuestCamCtrlCtor`) (vtable `0x08af4414`): `GameQuestCamModeBaseCtor`, path set
   `desc+0x50` -> `+0x100`, no attachment (`+0x104`/`+0x108`), default direction `posZ` of `g_gameQuestCamPathModeAxisConsts`, `regVec` zeroed (VFPU bank C720),
   distances 800/800, factor 1.5, count 5. */

GameQuestCamPathMode *GameQuestCamPathModeCtor(GameQuestCamPathMode *self, void *desc)

{
  GameQuestCamModeBaseCtor(&self->base,desc);
  (self->base).base.base.vtbl = g_gameQuestCamPathModeVtbl;
  self->pathSet = ((GameQuestCamPathModeDesc *)desc)->pathSet;
  self->segment = NULL;
  self->prevSegment = NULL;
  self->defaultDir = g_gameQuestCamPathModeAxisConsts.posZ;
  self->regVec.x = 0.0f;
  self->regVec.y = 0.0f;
  self->regVec.z = 0.0f;
  self->regVec.w = 0.0f;
  self->zero134 = 0.0f;
  self->pullFactor = 1.5f;
  self->retries = 5;
  self->targetDist = 800.0f;
  self->pitchDist = 800.0f;
  (self->base).kind = 0;
  return self;
}

