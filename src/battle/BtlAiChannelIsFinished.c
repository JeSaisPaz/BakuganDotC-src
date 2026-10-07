// bdc 0x0889363c BtlAiChannelIsFinished
#include "bdc.h"

/* Returns 1 when a command channel of `BtlAi` has step counter 0 and its done byte
   set, else 0. */
s32 BtlAiChannelIsFinished(BtlAiChannel *self)
{
    return self->step == 0 && self->done != 0;
}
