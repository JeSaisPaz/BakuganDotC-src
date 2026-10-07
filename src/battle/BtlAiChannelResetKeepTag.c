// bdc 0x08894bdc BtlAiChannelResetKeepTag
#include "bdc.h"

/* Resets a command channel of `BtlAi` with `BtlAiChannelReset` but keeps its
   s16 rule `tag` (read before the reset and written back after it). */
void BtlAiChannelResetKeepTag(BtlAiChannel *self)
{
    s16 tag = self->tag;

    BtlAiChannelReset(self);
    self->tag = tag;
}
