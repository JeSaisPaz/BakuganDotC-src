// bdc 0x088f10e4 GameEventOp82Nop
#include "bdc.h"

/* Handler of event opcode 0x82 (`GameEvent470ExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp82Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
