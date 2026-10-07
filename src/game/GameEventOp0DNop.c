// bdc 0x088ec098 GameEventOp0DNop
#include "bdc.h"

/* Handler of event opcode 0x0d (GameEventExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOp0DNop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
