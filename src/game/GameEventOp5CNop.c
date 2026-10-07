// bdc 0x088ee044 GameEventOp5CNop
#include "bdc.h"

/* Handler of event opcode 0x5c (GameEventExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOp5CNop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
