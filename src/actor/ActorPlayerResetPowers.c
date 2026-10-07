// bdc 0x088e13c8 ActorPlayerResetPowers
#include "bdc.h"

/* Cancels both powers (`ActorPlayerCancelPowers`) and refills both gauges (`+0x3bc`, `+0x3c0` =
   1.0). Called by `GameFieldPhaseMain`. */

void ActorPlayerResetPowers(ActorPlayer *self)

{
  ActorPlayerCancelPowers(self);
  self->gaugeB = 1.0f;
  self->gaugeA = 1.0f;
  return;
}

