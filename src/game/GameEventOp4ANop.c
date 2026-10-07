// bdc 0x088edf44 GameEventOp4ANop
#include "bdc.h"

/* Handler of event opcode 0x4a (GameEventExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOp4ANop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
