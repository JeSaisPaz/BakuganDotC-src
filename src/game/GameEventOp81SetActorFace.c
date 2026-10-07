// bdc 0x088f1090 GameEventOp81SetActorFace
#include "bdc.h"

/* Handler of event opcode 0x81 (`GameEvent470ExecCommand`): sets the face expression `arg` of the
   actor of entry `flag` (`ActorSetFaceExpression`). */

void GameEventOp81SetActorFace(GameEvent470 *self, u8 flag, s16 arg)

{
  u32 idx;

  idx = flag;
  if (idx == 100) {
    idx = self->talkPartner;
  }
  ActorSetFaceExpression((Actor *)g_gameFieldCharSet->actors[self->actorMap[idx]], (u8)arg);
}
