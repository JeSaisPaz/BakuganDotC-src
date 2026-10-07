// bdc 0x088ee07c GameEventOp63Nop
#include "bdc.h"

/* Handler of event opcode 0x63 (GameEventExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOp63Nop(void *ev,u8 flag,s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
