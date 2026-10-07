// bdc 0x088f8ee0 GameQuestCamLookSpringUpdate
#include "bdc.h"

/* Slot 5 of the look-at spring: spring acceleration (`GameQuestCamSpringAccelerate`, strength 1.1) and integration
   (`GameQuestCamIntegrate`). */

void GameQuestCamLookSpringUpdate(float dt, GameQuestCamLookSpring *self)

{
  GameQuestCamSpringAccelerate(dt,1.1f,&self->base,&(self->point).x);
  GameQuestCamIntegrate(dt,&self->base,&(self->point).x,&(self->base).vel.x,&(self->base).accel.x);
  return;
}

