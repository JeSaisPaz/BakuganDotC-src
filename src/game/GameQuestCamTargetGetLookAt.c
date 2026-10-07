// bdc 0x08a2c800 GameQuestCamTargetGetLookAt
#include "bdc.h"

/* Entry 2 (`+0x14`) of the quest camera spring point vtable `0x08af6e58`
   (`GameQuestCamTargetCtor`), inherited by the camera modes: returns the look-at vector `cam +
   0x60`. */

float *GameQuestCamTargetGetLookAt(GameQuestCamTarget *self)

{
  return &(self->point).x;
}

