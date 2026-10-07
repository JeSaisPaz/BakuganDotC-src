// bdc 0x088ee00c GameEventOp55Nop
#include "bdc.h"

/* Handler of event opcode 0x55 (GameEventExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOp55Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
