// bdc 0x088ee01c GameEventOp57Nop
#include "bdc.h"

/* Handler of event opcode 0x57 (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */

void GameEventOp57Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
