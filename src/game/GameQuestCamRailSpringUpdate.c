// bdc 0x088f9320 GameQuestCamRailSpringUpdate
#include "bdc.h"

/* Update of the rail spring: spring acceleration (`GameQuestCamSpringAccelerate`, strength 1.1) and integration
   (`GameQuestCamIntegrate`). */

void GameQuestCamRailSpringUpdate(float dt, void *spring)
{
  GameQuestCamSpring *base = (GameQuestCamSpring *)spring;
  float *point = (float *)(base + 1);

  GameQuestCamSpringAccelerate(dt, 1.1f, base, point);
  GameQuestCamIntegrate(dt, base, point, &base->vel.x, &base->accel.x);
}
