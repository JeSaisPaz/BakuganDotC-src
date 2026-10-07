// bdc 0x08895d00 BtlAiChannelSetCommand
#include "bdc.h"

/* Queues command `cmd` on a command channel of `BtlAi`: clears the channel state
   (`BtlAiChannelClearState`), drops a pending idle command (0x1b) when `cmd` is not 0, and stores
   `cmd`, the end condition kind, value `value`, comparison `condOp` and the rule tag `tag`. For
   condition kind 3 (timer) it also sets `cmdLimit = (int)value * 1/30` seconds, `cmdElapsed = 0`
   and `cmdExpired` when the limit is <= 0. */
void BtlAiChannelSetCommand(float value, BtlAiChannel *self, s32 cmd, u8 condKind, s32 condOp, s16 tag)
{
    BtlAiChannelClearState(self);
    if (self->pendingCmd == 0x1b && cmd != 0) {
        self->pendingCmd = 0;
    }
    self->condKind = condKind;
    self->cmd = cmd;
    self->condValue = value;
    self->condOp = condOp;
    self->tag = tag;
    if (self->condKind == 3) {
        float limit = (float)(s32)self->condValue * 0.0333333351f;

        self->cmdElapsed = 0.0f;
        self->cmdLimit = limit;
        self->cmdExpired = limit <= 0.0f;
    }
}
