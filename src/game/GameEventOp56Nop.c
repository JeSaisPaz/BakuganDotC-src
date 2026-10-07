// bdc 0x088ee014 GameEventOp56Nop
#include "bdc.h"

/* Handler of event opcode 0x56 (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp56Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
