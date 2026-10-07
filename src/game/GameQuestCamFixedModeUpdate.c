// bdc 0x088f67d0 GameQuestCamFixedModeUpdate
#include "bdc.h"

/* Update (slot 6) of the fixed-camera mode: spring-accelerates toward the camera entry's point
   (`GameQuestCamSpringAccelerateByDistance`, strength 1.2f, distances 400/1200), copies the
   entry's near/far values (`+0x50`/`+0x54`) to the camera, applies it when the entry's flag `+0x61`
   is set (`GameQuestCamResolveCollision`) and integrates the spring (`GameQuestCamIntegrate`).
    */

void GameQuestCamFixedModeUpdate(float dt, GameQuestCamFixedMode *self)

{
  ScePspFVector4 *pos;
  
  pos = &(self->base).point;
  GameQuestCamSpringAccelerateByDistance
            (dt,1.2f,self->entry->distance,400.0f,1200.0f,(GameQuestCamSpring *)self,&pos->x);
  ((self->base).base.ctrl)->extra[1] = self->entry->extra1;
  ((self->base).base.ctrl)->extra[0] = self->entry->extra0;
  if (self->entry->collide != '\0') {
    GameQuestCamResolveCollision(dt,(GameQuestCamSpring *)self);
  }
  GameQuestCamIntegrate
            (dt,(GameQuestCamSpring *)self,&pos->x,&(self->base).base.vel.x,
             &(self->base).base.accel.x);
  return;
}

