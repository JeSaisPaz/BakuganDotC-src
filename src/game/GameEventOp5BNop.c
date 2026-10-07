// bdc 0x088ee03c GameEventOp5BNop
#include "bdc.h"

/* Handler of event opcode 0x5b (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp5BNop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
