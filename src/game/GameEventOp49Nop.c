// bdc 0x088edf3c GameEventOp49Nop
#include "bdc.h"

/* Handler of event opcode 0x49 (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */

void GameEventOp49Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
