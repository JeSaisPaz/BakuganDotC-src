// bdc 0x08895dcc BtlAiChannelAdvanceStep
#include "bdc.h"

/* Increments the step counter of a command channel of `BtlAi`. */
void BtlAiChannelAdvanceStep(BtlAiChannel *self)
{
    self->step++;
}
