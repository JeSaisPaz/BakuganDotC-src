// bdc 0x088ee04c GameEventOp5DNop
#include "bdc.h"

/* Handler of event opcode 0x5d (GameEventExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOp5DNop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
