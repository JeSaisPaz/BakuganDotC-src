// bdc 0x088ec0a8 GameEventOp0FNop
#include "bdc.h"

/* Handler of event opcode 0x0f (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp0FNop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
