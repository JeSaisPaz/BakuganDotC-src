// bdc 0x088a5414 ActorStageObjEggCrystalElementUpdate
#include "bdc.h"

/* Update method of the attribute egg crystal (vtable `0x08af25c4` slot 7,
   `ActorStageObjEggCrystalElementCtor`): on the first call marks it broken (`+900`, `+0x282`),
   runs the break virtual `+0x5c` (`ActorStageObjEggCrystalBreak`) and rebuilds the matrix — the
   crystal shatters immediately to release the Bakugan. */

typedef struct StageObjVtable {
  u8 _unk00[0x58];
  s16 thisAdjust; /* +0x58 */
  s16 _pad5a;
  void (*breakFn)(void *); /* +0x5c */
} StageObjVtable;

void ActorStageObjEggCrystalElementUpdate(ActorStageObjEggCrystal *self)
{
  const StageObjVtable *vtable;

  if (self->broken == 0) {
    vtable = (const StageObjVtable *)self->base.base.base.vtable;
    self->broken = 1;
    self->base.removeRequest = 1;
    vtable->breakFn((u8 *)self + vtable->thisAdjust);
    ActorStageObjEggCrystalUpdateTransform(self);
  }
}
