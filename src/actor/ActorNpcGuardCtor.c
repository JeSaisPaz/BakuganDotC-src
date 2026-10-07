// bdc 0x088e8c74 ActorNpcGuardCtor
#include "bdc.h"

/* Constructor of the guard class (models 0x4e/0x50 `npc_sm_we/st`, 0x460 bytes, vtable
   `0x08af4024`; also the base of `ActorNpcCloakCtor`): `ActorNpcCtor`, hearing radius 200,
   alert view (`ActorNpcGuardSetAlertView`), speed 1.3/1.4/1.5 for models 0x4e/0x4f/0x50, no
   specular, ball position zeroed, look mode `+0x450 = 2`, sound id `+0x430 = 0x2757`, home heading = current heading. */

void *ActorNpcGuardCtor(ActorNpcGuard *self, s32 modelId)

{
  ActorNpcCtor(&self->base,modelId);
  (self->base).base.base.base.vtable = g_actorNpcGuardVtbl;
  (self->base).hearRadius = 200.0f;
  ActorNpcGuardSetAlertView(&self->base,1);
  if (modelId < 0x4f) {
    if (0x4d < modelId) {
      (self->base).speed = 1.3f;
    }
  }
  else if (modelId < 0x50) {
    (self->base).speed = 1.4f;
  }
  else if (modelId < 0x51) {
    (self->base).speed = 1.5f;
  }
  GfxModelSetSpecular(0.0f,(GfxModel *)self,&g_colorBlack.x,NULL);
  (self->base).timer = 0;
  self->lookMode = 2;
  self->ballPos[0] = 0.0f;
  self->ballPos[1] = 0.0f;
  self->ballPos[2] = 0.0f;
  self->ballPos[3] = 0.0f;
  (self->base).voiceId = 0x2757;
  (self->base).homeHeading = (self->base).base.base.rot[1];
  return self;
}

