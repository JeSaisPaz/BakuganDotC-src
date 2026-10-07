// bdc 0x088f18ac GameEventOpAANop
#include "bdc.h"

/* Handler of event opcode 0xaa (GameEvent470ExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOpAANop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
