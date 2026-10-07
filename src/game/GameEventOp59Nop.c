// bdc 0x088ee02c GameEventOp59Nop
#include "bdc.h"

/* Handler of event opcode 0x59 (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp59Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
