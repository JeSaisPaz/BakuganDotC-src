// bdc 0x088ee064 GameEventOp60Nop
#include "bdc.h"

/* Handler of event opcode 0x60 (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp60Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
