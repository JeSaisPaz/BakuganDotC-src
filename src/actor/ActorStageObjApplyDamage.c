// bdc 0x088ac998 ActorStageObjApplyDamage
#include "bdc.h"

/* Subtracts `damage` from the object's HP `+0x200` while it is positive. */

void ActorStageObjApplyDamage(float damage, ActorStageObjBase *self)

{
  if (0 < self->hp) {
    self->hp = (int)((float)self->hp - damage);
  }
  return;
}

