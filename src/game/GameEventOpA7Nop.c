// bdc 0x088f1860 GameEventOpA7Nop
#include "bdc.h"

/* Handler of event opcode 0xa7 (`GameEvent470ExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOpA7Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
