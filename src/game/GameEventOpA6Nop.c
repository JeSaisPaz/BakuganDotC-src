// bdc 0x088f1858 GameEventOpA6Nop
#include "bdc.h"

/* Handler of event opcode 0xa6 (`GameEvent470ExecCommand`): does nothing (unused or
   compiled-out opcode). */

void GameEventOpA6Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
