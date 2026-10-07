// bdc 0x088ec090 GameEventOp0CNop
#include "bdc.h"

/* Handler of event opcode 0x0c (`GameEventExecCommand`): does nothing (unused or compiled-out
   opcode). */
void GameEventOp0CNop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
