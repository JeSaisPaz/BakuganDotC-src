// bdc 0x088f18b4 GameEventOpABNop
#include "bdc.h"

/* Handler of event opcode 0xab (GameEvent470ExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOpABNop(void *ev,u8 flag,s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
