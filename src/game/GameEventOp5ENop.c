// bdc 0x088ee054 GameEventOp5ENop
#include "bdc.h"

/* Handler of event opcode 0x5e (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp5ENop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
