// bdc 0x088f0638 GameEventOp6ESetActorZ
#include "bdc.h"

/* Handler of event opcode 0x6e (`GameEvent470ExecCommand`): sets the end-position z (`+0x14`) of
   event actor `flag`. */

void GameEventOp6ESetActorZ(GameEvent470 *self, u8 flag, s16 arg)

{
  u32 idx = flag;
  s64 scaled;

  if (idx == 100) {
    idx = self->talkPartner;
  }
  scaled = (s64)arg << 12;
  self->actors[self->actorMap[idx]].endPos[2] = (s32)__divdi3(scaled, g_gameFixedScale);
}
