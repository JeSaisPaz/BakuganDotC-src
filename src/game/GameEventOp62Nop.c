// bdc 0x088ee074 GameEventOp62Nop
#include "bdc.h"

/* Handler of event opcode 0x62 (GameEventExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOp62Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
