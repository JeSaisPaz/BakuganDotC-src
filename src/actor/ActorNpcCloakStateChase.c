// bdc 0x088e5608 ActorNpcCloakStateChase
#include "bdc.h"

/* Chase state of the cloaked guard class (model 0x4f, `ActorNpcCloakCtor`, vtable `0x08af39e4`)
   (slot 39): on entry shows the reveal effect 0x44 and switches to mode 1 (visible), then runs the
   common `ActorNpcStateCatchPlayer`. */

void ActorNpcCloakStateChase(ActorNpcCloak *self)

{
  if (self->mode != 1) {
    GfxEffectSpawnAttached(g_worldEffectMgr,0x44,(self->base).base.base.mtx + 0xc);
    ActorNpcCloakSetMode(self,1);
  }
  ActorNpcStateCatchPlayer((ActorNpc *)self);
  return;
}

