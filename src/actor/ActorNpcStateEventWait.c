// bdc 0x088e7b1c ActorNpcStateEventWait
#include "bdc.h"

/* AI state 9 of the field NPC/guard classes (base `ActorNpcCtor`) (slot 44): waits until the
   player's event flag (motion helper `+0xc`) clears, then restores the saved state `+0x42c`. */

void ActorNpcStateEventWait(ActorNpc *self)
{
  Actor *player = (Actor *)ActorFindPlayer();

  if (((BtlInput *)player->input)->disabled == 0) {
    self->aiState = self->savedState;
  }
}
