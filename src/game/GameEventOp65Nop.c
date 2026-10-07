// bdc 0x088ee08c GameEventOp65Nop
#include "bdc.h"

/* Handler of event opcode 0x65 (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */

void GameEventOp65Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
