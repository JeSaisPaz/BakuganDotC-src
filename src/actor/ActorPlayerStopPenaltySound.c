// bdc 0x088e1530 ActorPlayerStopPenaltySound
#include "bdc.h"

/* Stops the penalty sound `+0x3cc` if one is playing and resets the handle to -1. */

void ActorPlayerStopPenaltySound(ActorPlayer *self)
{
    if (self->penaltySound != 0xffffffff) {
        if (!SndHasManager()) {
            self->penaltySound = 0xffffffff;
        } else {
            SndManagerStop(SndGetManager(), self->penaltySound);
            self->penaltySound = 0xffffffff;
        }
    }
}
