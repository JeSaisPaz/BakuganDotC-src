// bdc 0x088f13c8 GameEventOp96SetTransitionActor
#include "bdc.h"

/* Handler of event opcode 0x96 (`GameEvent470ExecCommand`): stores the transition actor index
   `flag` (100 = talk partner) in `0x08b00dc6`. */

void GameEventOp96SetTransitionActor(GameEvent470 *self, u8 flag, s16 arg)

{
  if (flag == 'd') {
    flag = self->talkPartner;
  }
  g_gameEventTransitionActor = flag;
  return;
}

