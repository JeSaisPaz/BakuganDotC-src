// bdc 0x088edf4c GameEventOp4BNop
#include "bdc.h"

/* Handler of event opcode 0x4b (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp4BNop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
