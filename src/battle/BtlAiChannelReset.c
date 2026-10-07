// bdc 0x0888cfa4 BtlAiChannelReset
#include "bdc.h"

/* Resets a 300-byte command channel of `BtlAi`: sets the current and the pending
   command to 0x1b (idle), then clears the remaining state with `BtlAiChannelClearState`. */
void BtlAiChannelReset(BtlAiChannel *self)
{
    self->cmd = 0x1b;
    self->pendingCmd = 0x1b;
    BtlAiChannelClearState(self);
}
