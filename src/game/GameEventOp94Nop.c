// bdc 0x088f1388 GameEventOp94Nop
#include "bdc.h"

/* Handler of event opcode 0x94 (`GameEvent470ExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp94Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
