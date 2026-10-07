// bdc 0x088e1dec ActorPlayerSetLoopSoundsPaused
#include "bdc.h"

/* Pauses (`paused` != 0: stops) or resumes (restarts) the player's looping sounds: the scan loop
   `0x2c0002b` (`scanSound`, while `scan`) and the recharge loop `0x2c00038` (`loopSound`, while
   `loopSoundPlaying`). Resuming stores the new handles. */

void ActorPlayerSetLoopSoundsPaused(ActorPlayer *self, u8 paused)
{
    if (paused != 0) {
        if (self->scan != 0 && SndHasManager()) {
            SndManagerStop(SndGetManager(), self->scanSound);
        }
        if (self->loopSoundPlaying != 0 && SndHasManager()) {
            SndManagerStop(SndGetManager(), self->loopSound);
        }
    } else {
        if (self->scan != 0 && SndHasManager()) {
            self->scanSound = SndManagerPlay(SndGetManager(), 0x2c0002b, 0, 0);
        }
        if (self->loopSoundPlaying != 0 && SndHasManager()) {
            self->loopSound = SndManagerPlay(SndGetManager(), 0x2c00038, 0, 0);
        }
    }
}
