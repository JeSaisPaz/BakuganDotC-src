// bdc 0x088ee034 GameEventOp5ANop
#include "bdc.h"

/* Handler of event opcode 0x5a (GameEventExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOp5ANop(void *ev,u8 flag,s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
