// bdc 0x088f869c GameQuestCamPointModeUpdateEye
#include "bdc.h"

/* Slot 6 of the point camera mode: spring-accelerates toward the entry
   (`GameQuestCamSpringAccelerateByDistance`, strength 1, 400/1200), copies the entry's near/far
   values to the camera, refreshes the look-at unless snapping, applies the camera when allowed and
   integrates (`GameQuestCamIntegrate`, `GameQuestCamModeCaptureTargetPos`). */

void GameQuestCamPointModeUpdateEye(float dt, GameQuestCamPointMode *self)

{
  ScePspFVector4 *pos;
  
  pos = &(self->base).base.point;
  GameQuestCamSpringAccelerateByDistance
            (dt,1.0f,self->entry->distance,400.0f,1200.0f,(GameQuestCamSpring *)self,&pos->x);
  ((self->base).base.base.ctrl)->extra[1] = self->entry->extra1;
  ((self->base).base.base.ctrl)->extra[0] = self->entry->extra0;
  if (((self->base).base.base.ctrl)->snap == '\0') {
    GameQuestCamModeResetAttached(&self->base);
  }
  if (self->entry->collide != '\0') {
    GameQuestCamResolveCollision(dt,(GameQuestCamSpring *)self);
  }
  GameQuestCamIntegrate
            (dt,(GameQuestCamSpring *)self,&pos->x,&(self->base).base.base.vel.x,
             &(self->base).base.base.accel.x);
  GameQuestCamModeCaptureTargetPos(&self->base);
  return;
}

