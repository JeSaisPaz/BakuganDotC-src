// bdc 0x088f1208 GameEventOp8BNop
#include "bdc.h"

/* Handler of event opcode 0x8b (GameEvent470ExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOp8BNop(void *ev,u8 flag,s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
