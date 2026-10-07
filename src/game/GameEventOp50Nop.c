// bdc 0x088edfbc GameEventOp50Nop
#include "bdc.h"

/* Handler of event opcode 0x50 (GameEventExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOp50Nop(void *ev,u8 flag,s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
