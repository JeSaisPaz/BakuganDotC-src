// bdc 0x088ec0a0 GameEventOp0ENop
#include "bdc.h"

/* Handler of event opcode 0x0e (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp0ENop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
