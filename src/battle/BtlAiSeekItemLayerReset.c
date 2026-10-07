// bdc 0x08a2a3a8 BtlAiSeekItemLayerReset
#include "bdc.h"

/* Reset method (entry 1) of the BtlAiSeekItemLayer behaviour layer of the CPU AI (`BtlAiCtor`): resets the
   base layer like `BtlAiLayerReset` and additionally clears the target item. */
void BtlAiSeekItemLayerReset(BtlAiSeekItemLayer *self)
{
    self->base.state = 0;
    self->base.active = 0;
    self->base.flags = 0;
    self->base.cmd.state = 0;
    self->base.cmd.timerLimit = 0.0f;
    self->base.cmd.timerElapsed = 0.0f;
    self->base.cmd.timerExpired = 1;
    self->base.cmd.flags = 0;
    self->base.cmd.arg = 0;
    self->base.cmd.argF = 0.0f;
    self->base.cmd.finished = 0;
    self->base.cmd.failed = 0;
    self->item = NULL;
}
