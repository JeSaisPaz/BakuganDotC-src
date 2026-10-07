// bdc 0x088f8a9c GameQuestCamEyeSpringUpdate
#include "bdc.h"

/* Slot 6 of the eye spring: spring acceleration (`GameQuestCamSpringAccelerate`, strength 1.9) toward `+0x60` and
   integration (`GameQuestCamIntegrate`). */

void GameQuestCamEyeSpringUpdate(float dt, GameQuestCamEyeSpring *self)

{
  GameQuestCamSpringAccelerate(dt,1.9f,&self->base,&(self->point).x);
  GameQuestCamIntegrate(dt,&self->base,&(self->point).x,&(self->base).vel.x,&(self->base).accel.x);
  return;
}

