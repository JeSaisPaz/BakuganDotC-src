// bdc 0x088ee024 GameEventOp58Nop
#include "bdc.h"

/* Handler of event opcode 0x58 (GameEventExecCommand): does nothing (unused or compiled-out
      opcode). */
void GameEventOp58Nop(void *ev,u8 flag,s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
