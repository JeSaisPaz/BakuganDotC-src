// bdc 0x088f11f8 GameEventOp89Nop
#include "bdc.h"

/* Handler of event opcode 0x89 (`GameEvent470ExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp89Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
