// bdc 0x088ee094 GameEventOp66Nop
#include "bdc.h"

/* Handler of event opcode 0x66 (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp66Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
