// bdc 0x088f1ac0 GameEventOpB4Nop
#include "bdc.h"

/* Handler of event opcode 0xb4 (`GameEvent470ExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOpB4Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
