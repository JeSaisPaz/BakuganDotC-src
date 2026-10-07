// bdc 0x088f17b0 GameEventOpA3Nop
#include "bdc.h"

/* Handler of event opcode 0xa3 (`GameEvent470ExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOpA3Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
