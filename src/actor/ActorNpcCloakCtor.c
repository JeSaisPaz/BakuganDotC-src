// bdc 0x088e54a4 ActorNpcCloakCtor
#include "bdc.h"

/* Constructor of the cloaked guard class (model 0x4f, vtable `0x08af39e4`), 0x470 bytes: runs the
   guard constructor `ActorNpcGuardCtor` with the caller's `modelId` (passed through in a1), installs
   the vtable, clears `mode`, `flickerTimer`, `flickerFrame`, `flickerPhase` and starts hidden
   (`ActorNpcCloakSetMode` 0). Returns `self`. */

void *ActorNpcCloakCtor(ActorNpcCloak *self, s32 modelId)

{
  ActorNpcGuardCtor(&self->base, modelId);
  (self->base).base.base.base.base.vtable = &g_actorNpcCloakVtbl;
  self->mode = 0;
  self->flickerTimer = 0;
  self->flickerFrame = 0;
  self->flickerPhase = 0;
  ActorNpcCloakSetMode(self, 0);
  return self;
}
