// bdc 0x088e5d48 ActorNpcUpdatePhase
#include "bdc.h"

/* Vtable slot 7 of the field NPC/guard classes (base `ActorNpcCtor`): while the setup phase
   `+0x3a4` is 0..2, stops the NPC (zeroes the velocity `+0x80` from the VFPU bank constant C720)
   and runs the phase handler from the pointer-to-member table `0x08a98cd8` (phase 0 =
   `ActorNpcPhase00ApplyPlacement`, phase 1 = virtual slot 34 `ActorNpcUpdate`, phase 2 =
   `ActorNpcPhase02Nop`); then calls virtual slot 48 (`+0x184`). */

void ActorNpcUpdatePhase(ActorNpc *self)
{
  const VtblEntry *vt;

  if (self->phase >= 0 && self->phase < 3) {
    self->base.base.velocity[0] = 0.0f;
    self->base.base.velocity[1] = 0.0f;
    self->base.base.velocity[2] = 0.0f;
    self->base.base.velocity[3] = 0.0f;
    switch (self->phase) {
    case 0:
      ActorNpcPhase00ApplyPlacement(self);
      break;
    case 1: {
      const VtblEntry *v = (const VtblEntry *)self->base.base.base.vtable;
      ((void (*)(void *))v[34].fn)((u8 *)self + v[34].delta);
      break;
    }
    default:
      ActorNpcPhase02Nop(self);
      break;
    }
  }
  vt = (const VtblEntry *)self->base.base.base.vtable;
  ((void (*)(void *))vt[48].fn)((u8 *)self + vt[48].delta);
}
