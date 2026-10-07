// bdc 0x088f04f8 GameEventOp6CSetActorX
#include "bdc.h"

/* Handler of event opcode 0x6c (`GameEvent470ExecCommand`): sets the end-position x (`+0xc`) of
   event actor `flag` (index into the field character set; 100 = the current talk partner `+0x2d4`;
   record `ev+0x284 + map[+0x292+i]*0x4c`) from `arg` (20.12 fixed point). */

void GameEventOp6CSetActorX(GameEvent470 *self, u8 flag, s16 arg)

{
  u32 idx = flag;
  s64 scaled;

  if (idx == 100) {
    idx = self->talkPartner;
  }
  scaled = (s64)arg << 12;
  self->actors[self->actorMap[idx]].endPos[0] = (s32)__divdi3(scaled, g_gameFixedScale);
}
