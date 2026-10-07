// bdc 0x088a5414 ActorStageObjEggCrystalElementUpdate
#include "bdc.h"

/* Update method of the attribute egg crystal (vtable `0x08af25c4` slot 7,
   `ActorStageObjEggCrystalElementCtor`): on the first call marks it broken (`+900`, `+0x282`),
   runs the break virtual `+0x5c` (`ActorStageObjEggCrystalBreak`) and rebuilds the matrix — the
   crystal shatters immediately to release the Bakugan. */

void ActorStageObjEggCrystalElementUpdate(ActorStageObjEggCrystal *self)
{
  const VtblEntry *breakFn;

  if (self->broken == 0) {
    breakFn = &((const VtblEntry *)self->base.base.base.vtable)[11];
    self->broken = 1;
    self->base.removeRequest = 1;
    ((void (*)(void *))breakFn->fn)((u8 *)self + breakFn->delta);
    ActorStageObjEggCrystalUpdateTransform(self);
  }
}
