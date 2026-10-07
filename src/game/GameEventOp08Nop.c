// bdc 0x088ebe60 GameEventOp08Nop
#include "bdc.h"

/* Handler of event opcode 0x08 (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp08Nop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
