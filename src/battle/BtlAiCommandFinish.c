// bdc 0x08897e84 BtlAiCommandFinish
#include "bdc.h"

/* Ends an executing command of `BtlAi`: sets its state to 4 (done) and releases
   pad action bits 0x10 and 0x10000. */
void BtlAiCommandFinish(BtlAi *self, BtlAiCommand *cmd)
{
    cmd->state = 4;
    self->pad.cur.aiActions &= 0xfffeffef;
}
