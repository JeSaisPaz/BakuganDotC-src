// bdc 0x088edffc GameEventOp53Nop
#include "bdc.h"

/* Handler of event opcode 0x53 (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp53Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
