// bdc 0x08895db8 BtlAiChannelHasPending
#include "bdc.h"

/* Returns true when a command channel of `BtlAi` has a pending command that differs
   from the current one. */
s32 BtlAiChannelHasPending(BtlAiChannel *self)
{
    return self->pendingCmd != self->cmd;
}
