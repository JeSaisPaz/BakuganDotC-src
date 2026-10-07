// bdc 0x088571f8 ActorCrystalScrollUvs
#include "bdc.h"

/* Advances the crystal's four texture scrollers `+0x800`, `+0x810`, `+0x820`, `+0x830`
   (`ActorCrystalUvScrollStep`); called by `ActorCrystalUpdate`. */

void ActorCrystalScrollUvs(ActorCrystal *self)
{
  ActorCrystalUvScrollStep((float *)&self->uvScrollers[0x00]);
  ActorCrystalUvScrollStep((float *)&self->uvScrollers[0x10]);
  ActorCrystalUvScrollStep((float *)&self->uvScrollers[0x20]);
  ActorCrystalUvScrollStep((float *)&self->uvScrollers[0x30]);
}
