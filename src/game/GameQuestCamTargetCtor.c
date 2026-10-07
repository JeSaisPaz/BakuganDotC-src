// bdc 0x088fc730 GameQuestCamTargetCtor
#include "bdc.h"

/* Constructor of the quest camera spring point with a look-at (vtable `0x08af6e58`): runs
   `GameQuestCamSpringCtor` and copies the look-at vector `desc+0x30` into `point`. Returns
   `self`. */

typedef struct CamTargetDesc {
  GameQuestCamSpringDesc spring;
  ScePspFVector4 lookAt;
} CamTargetDesc;

GameQuestCamTarget *GameQuestCamTargetCtor(GameQuestCamTarget *self, void *desc)
{
  GameQuestCamSpringCtor(&self->base, desc);
  self->base.vtbl = g_gameQuestCamTargetVtbl;
  self->point = ((CamTargetDesc *)desc)->lookAt;
  return self;
}
