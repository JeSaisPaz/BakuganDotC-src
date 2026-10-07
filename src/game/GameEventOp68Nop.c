// bdc 0x088f0438 GameEventOp68Nop
#include "bdc.h"

/* Handler of event opcode 0x68 (GameEvent470ExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOp68Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
