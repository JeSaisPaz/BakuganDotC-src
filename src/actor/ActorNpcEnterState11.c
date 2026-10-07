// bdc 0x088e5e2c ActorNpcEnterState11
#include "bdc.h"

/* Puts an NPC into AI state 11 with setup phase 1: clears the caught flags and sub-steps, restores
   the turn rate 0.035 and removes the head effects. Called by `GameFieldPhaseMain`. */

void ActorNpcEnterState11(ActorNpc *self)
{
    self->base.alerted = 0;
    self->base.detected = 0;
    self->phase = 1;
    self->aiState = 0xb;
    self->subStep = 0;
    self->base.stuckFrames = 0;
    self->turnRate = 0.034906585f;
    ActorNpcShowHeadEffect(self, -1, 1, 1);
}
