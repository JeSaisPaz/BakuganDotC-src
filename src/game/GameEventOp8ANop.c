// bdc 0x088f1200 GameEventOp8ANop
#include "bdc.h"

/* Handler of event opcode 0x8a (GameEvent470ExecCommand): does nothing (unused or compiled-out
   opcode). */
void GameEventOp8ANop(void *ev, u8 flag, s16 arg)
{
    (void)ev;
    (void)flag;
    (void)arg;
}
