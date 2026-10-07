// bdc 0x088e8898 ActorNpcSwitchRobotUpdateFeet
#include "bdc.h"

/* Vtable slot 49 of the switch robot (model 0x53, `ActorNpcSwitchRobotCtor`, vtable
   `0x08af3d04`): while the jets are on (`+0x470`), copies the `R_Foot` and `L_Foot` bone positions
   to the effect anchors `+0x440`/`+0x450`. */

void ActorNpcSwitchRobotUpdateFeet(ActorNpcSwitchRobot *self)

{
  ScePspFVector4 tmp __attribute__((aligned(16)));
  
  if (self->feetEffects != '\0') {
    GfxModelGetNodeWorldPos((GfxModel *)self,&tmp,"R_Foot");
    self->footPos[0][0] = tmp.x;
    self->footPos[0][1] = tmp.y;
    self->footPos[0][2] = tmp.z;
    self->footPos[0][3] = tmp.w;
    GfxModelGetNodeWorldPos((GfxModel *)self,&tmp,"L_Foot");
    self->footPos[1][0] = tmp.x;
    self->footPos[1][1] = tmp.y;
    self->footPos[1][2] = tmp.z;
    self->footPos[1][3] = tmp.w;
  }
  return;
}

