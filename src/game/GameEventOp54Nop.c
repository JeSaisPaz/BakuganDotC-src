// bdc 0x088ee004 GameEventOp54Nop
#include "bdc.h"

/* Handler of event opcode 0x54 (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp54Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
