// bdc 0x08859dc8 ActorCrystalStartFade
#include "bdc.h"

/* Starts the crystal's fade sequence: step `+0xa74 = 1` and enable byte `+0xa78 = 1`, the two
   fields `ActorCrystalUpdateFade` runs on (it returns at once while `+0xa78` is clear). */

void ActorCrystalStartFade(ActorCrystal *self)

{
  self->fadeStep = 1;
  self->fadeEnabled = '\x01';
  return;
}

