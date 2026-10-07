// bdc 0x088f0598 GameEventOp6DSetActorY
#include "bdc.h"

/* Handler of event opcode 0x6d (`GameEvent470ExecCommand`): sets the end-position y (`+0x10`) of
   event actor `flag`. */

void GameEventOp6DSetActorY(GameEvent470 *self, u8 flag, s16 arg)

{
  u32 idx = flag;
  s64 scaled;

  if (idx == 100) {
    idx = self->talkPartner;
  }
  scaled = (s64)arg << 12;
  self->actors[self->actorMap[idx]].endPos[1] = (s32)__divdi3(scaled, g_gameFixedScale);
}
