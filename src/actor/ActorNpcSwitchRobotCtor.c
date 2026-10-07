// bdc 0x088e8130 ActorNpcSwitchRobotCtor
#include "bdc.h"

/* Constructor of the switch robot (model 0x53, 0x490 bytes, vtable `0x08af3d04`):
   `ActorNpcRobotCtor`, then clears `footPos`, `switchScroll`, `feetEffects`, `timer`,
   `motionFrame` and `motionEnd`. Returns `self`. */

void *ActorNpcSwitchRobotCtor(ActorNpcSwitchRobot *self, s32 modelId)

{
  ActorNpcRobotCtor(&self->base,modelId);
  (self->base).base.base.base.vtable = &g_actorNpcSwitchRobotVtbl;
  self->footPos[0][2] = 0.0f;
  self->footPos[0][1] = 0.0f;
  self->footPos[0][0] = 0.0f;
  self->footPos[0][3] = 0.0f;
  self->footPos[1][2] = 0.0f;
  self->footPos[1][1] = 0.0f;
  self->footPos[1][0] = 0.0f;
  self->footPos[1][3] = 0.0f;
  self->switchScroll[2] = 0.0f;
  self->switchScroll[1] = 0.0f;
  self->switchScroll[0] = 0.0f;
  self->switchScroll[3] = 0.0f;
  self->feetEffects = 0;
  self->timer = 0;
  self->motionFrame = 0.0f;
  self->motionEnd = 0.0f;
  return self;
}

