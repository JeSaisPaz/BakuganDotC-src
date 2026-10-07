// bdc 0x088f0f24 GameEventOp7DTurnBack
#include "bdc.h"

/* Handler of event opcode 0x7d (`GameEvent470ExecCommand`): restores the talk actor `b`'s
   original heading (`+0x44` -> target `+0x38`) with a rotate tween of `frames` frames. */

void GameEventOp7DTurnBack(GameEvent470 *self, s16 frames, s16 arg)

{
  self->actors[self->talkB].endRot[1] = self->actors[self->talkB].origRot[1];
  GameEvent470AddActorTween(self, (u16)frames, 1, self->talkB);
}
