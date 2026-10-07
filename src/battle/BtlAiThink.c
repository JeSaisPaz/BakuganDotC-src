// bdc 0x08897128 BtlAiThink
#include "bdc.h"

/* One think step of the CPU AI (`BtlAi`, run method of behaviour layer 4): runs
   `BtlAiRunReactionRules`, then `BtlAiUpdateTarget` and, when that returns non-zero, resets
   the move and attack channels (`channels[0]`/`channels[1]`, `BtlAiChannelReset`); then
   `BtlAiRunMoveRules` and `BtlAiRunAttackRules`. Finally `idleTime` grows by 1/30 s while
   both of those channels have pending command 0x1b (idle), else it is reset to 0. */
void BtlAiThink(BtlAi *self)
{
    BtlAiRunReactionRules(self);
    if (BtlAiUpdateTarget(self) != 0) {
        BtlAiChannelReset(&self->channels[0]);
        BtlAiChannelReset(&self->channels[1]);
    }
    BtlAiRunMoveRules(self);
    BtlAiRunAttackRules(self);
    if (self->channels[0].pendingCmd == 0x1b && self->channels[1].pendingCmd == 0x1b) {
        self->idleTime = self->idleTime + 0.0333333351f;
    } else {
        self->idleTime = 0.0f;
    }
}
