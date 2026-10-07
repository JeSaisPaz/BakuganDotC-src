// bdc 0x088f11a4 GameEventOp87Nop
#include "bdc.h"

/* Handler of event opcode 0x87 (`GameEvent470ExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp87Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
