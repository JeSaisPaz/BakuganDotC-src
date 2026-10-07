// bdc 0x088e8984 ActorNpcRobotCtor
#include "bdc.h"

/* Constructor of the robot classes (models 0x51..0x53 `npc_sr_we/mi/st`, 0x440 bytes, vtable
   `0x08af3e94`): `ActorNpcCtor`, per-model senses (0x52: hearing 125, view 30, half-angle 0.785,
   speed 1.95 and half scale; 0x51/0x53: hearing 250, view 67, half-angle 0.785 (0.698 on stage 8),
   speed 1.3), body capsule size (axis 15 / radius 7, or 7 / 3.5 for 0x52), model setup
   (`ActorNpcRobotSetupModel`) and sound id `+0x430 = 0x2a38`. Other model ids skip the senses and
   the capsule. Returns `self`.
   For 0x52 the halved scale's w lane is set to 0 (VFPU bank S713). */

void *ActorNpcRobotCtor(ActorNpc *self, s32 modelId)
{
  ActorNpcCtor(self, modelId);
  self->base.base.base.vtable = g_actorNpcRobotVtbl;
  if (modelId == 0x51 || modelId == 0x53) {
    self->hearRadius = 250.0f;
    self->viewDist = 67.0f;
    self->viewHalfAngle = 0.7853982f;
    if (g_scriptGlobalVars[1] == 8) {
      self->viewHalfAngle = 0.6981317f;
    }
    self->speed = 1.3f;
    self->base.collShape.start[0] = 0.0f;
    self->base.collShape.start[1] = 0.0f;
    self->base.collShape.start[2] = 0.0f;
    self->base.collShape.axis[0] = 0.0f;
    self->base.collShape.axis[1] = 15.0f;
    self->base.collShape.axis[2] = 0.0f;
    self->base.collShape.axisLen = 0.0f;
    self->base.collShape.radius = 7.0f;
    self->base.collShape.radiusSq = 7.0f * 7.0f;
    self->base.collShape.axisLen = __builtin_sqrtf(
        self->base.collShape.axis[0] * self->base.collShape.axis[0] +
        self->base.collShape.axis[1] * self->base.collShape.axis[1] +
        self->base.collShape.axis[2] * self->base.collShape.axis[2]);
  } else if (modelId == 0x52) {
    self->hearRadius = 125.0f;
    self->viewDist = 30.0f;
    self->viewHalfAngle = 0.7853982f;
    self->speed = 1.9499999f;
    /* scale.xyz *= 0.5; the w lane gets S713, the VFPU bank's 0 */
    self->base.base.scale[0] = self->base.base.scale[0] * 0.5f;
    self->base.base.scale[1] = self->base.base.scale[1] * 0.5f;
    self->base.base.scale[2] = self->base.base.scale[2] * 0.5f;
    self->base.base.scale[3] = 0.0f;
    self->base.collShape.start[0] = 0.0f;
    self->base.collShape.start[1] = 0.0f;
    self->base.collShape.start[2] = 0.0f;
    self->base.collShape.axis[0] = 0.0f;
    self->base.collShape.axis[1] = 7.0f;
    self->base.collShape.axis[2] = 0.0f;
    self->base.collShape.axisLen = 0.0f;
    self->base.collShape.radius = 3.5f;
    self->base.collShape.radiusSq = 3.5f * 3.5f;
    self->base.collShape.axisLen = __builtin_sqrtf(
        self->base.collShape.axis[0] * self->base.collShape.axis[0] +
        self->base.collShape.axis[1] * self->base.collShape.axis[1] +
        self->base.collShape.axis[2] * self->base.collShape.axis[2]);
  }
  ActorNpcRobotSetupModel(self);
  self->voiceId = 0x2a38;
  return self;
}
