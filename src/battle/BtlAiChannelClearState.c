// bdc 0x0888c254 BtlAiChannelClearState
#include "bdc.h"

/* Clears the state of a 300-byte command channel of `BtlAi` after the two command
   words (`cmd`/`pendingCmd` are left alone): zeroes the arguments, step/phase, timers, end
   condition and running-command fields, sets the three expired flags to 1, `tag` and `ruleId` to
   -1, and resets the three pad predicates to null member pointers (`g_btlAiNullPadConds`). */
void BtlAiChannelClearState(BtlAiChannel *self)
{
    self->arg0 = 0;
    self->arg1 = 0;
    self->argExtra[0] = 0;
    self->argExtra[1] = 0;
    self->argsSet = 0;
    self->toggle = 0;
    self->flagsExtra[0] = 0;
    self->flagsExtra[1] = 0;
    self->step = 0;
    self->phase = 0;
    self->phaseLimit = 0.0f;
    self->phaseElapsed = 0.0f;
    self->phaseExpired = 1;
    self->cmdLimit = 0.0f;
    self->cmdElapsed = 0.0f;
    self->cmdExpired = 1;
    self->condKind = 0;
    self->condValue = 0.0f;
    self->condOp = 0;
    self->tag = -1;
    self->padConds[0] = g_btlAiNullPadConds[0];
    self->padConds[1] = g_btlAiNullPadConds[1];
    self->padConds[2] = g_btlAiNullPadConds[2];
    self->exec.state = 0;
    self->exec.timerLimit = 0.0f;
    self->exec.timerElapsed = 0.0f;
    self->exec.timerExpired = 1;
    self->exec.flags = 0;
    self->exec.arg = 0;
    self->exec.argF = 0.0f;
    self->exec.finished = 0;
    self->exec.failed = 0;
    self->done = 0;
    self->ruleId = -1;
}
