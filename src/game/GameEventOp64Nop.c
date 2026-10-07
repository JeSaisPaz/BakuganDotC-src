// bdc 0x088ee084 GameEventOp64Nop
#include "bdc.h"

/* Handler of event opcode 0x64 (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp64Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
