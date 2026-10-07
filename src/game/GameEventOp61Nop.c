// bdc 0x088ee06c GameEventOp61Nop
#include "bdc.h"

/* Handler of event opcode 0x61 (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp61Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
