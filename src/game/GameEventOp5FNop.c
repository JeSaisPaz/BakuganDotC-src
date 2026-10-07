// bdc 0x088ee05c GameEventOp5FNop
#include "bdc.h"

/* Handler of event opcode 0x5f (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */

void GameEventOp5FNop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
