// bdc 0x088f16a0 GameEventOpA0SetSpawnY
#include "bdc.h"

/* Handler of event opcode 0xA0 (`GameEvent470ExecCommand`): sets the spawn y (s32 at `0x08b00d20`, word 1 of
   `g_gameUnlockFlags`) to ((s64)(value << 12) << 12) / g_gameFixedScale, as `GameEventOp9FSetSpawnX` does
   for x: the 16-bit `value` is shifted by 12 as a 32-bit word (20.12 fixed point), then by another 12 bits as
   a 64-bit value before the division. */

void GameEventOpA0SetSpawnY(GameEvent *self, s16 value, s16 unused)

{
  s64 scaled;

  scaled = (s64)((s32)value << 12) << 12;
  g_gameEventState.spawn.pos[1] = (s32)__divdi3(scaled, g_gameFixedScale);
}
