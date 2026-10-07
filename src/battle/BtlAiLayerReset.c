// bdc 0x08a2a368 BtlAiLayerReset
#include "bdc.h"

/* Resets one of the CPU AI's behaviour layers (base vtable entry 1; five layers are embedded in
   the AI object by `BtlAiCtor`): clears the layer state, active byte, flags and the embedded
   command (state, timers, flags, args, finished/failed) and marks the command timer expired. */
void BtlAiLayerReset(BtlAiLayer *self)
{
    self->state = 0;
    self->active = 0;
    self->flags = 0;
    self->cmd.state = 0;
    self->cmd.timerLimit = 0.0f;
    self->cmd.timerElapsed = 0.0f;
    self->cmd.timerExpired = 1;
    self->cmd.flags = 0;
    self->cmd.arg = 0;
    self->cmd.argF = 0.0f;
    self->cmd.finished = 0;
    self->cmd.failed = 0;
}
