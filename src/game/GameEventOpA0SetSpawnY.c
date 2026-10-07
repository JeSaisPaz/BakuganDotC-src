// bdc 0x088f16a0 GameEventOpA0SetSpawnY
#include "bdc.h"

/* Handler of event opcode 0xA0 (`GameEvent470ExecCommand`): sets the spawn y (s32 at `0x08b00d20`, word 1 of
   `g_gameUnlockFlags`) from the 16-bit value in the `value` argument (20.12 fixed point). */

void GameEventOpA0SetSpawnY(GameEvent *self, s16 value, s16 unused)

{
  s64 scaled;

  scaled = (s64)value << 12;
  ((s32 *)g_gameUnlockFlags)[1] = (s32)__divdi3(scaled, g_gameFixedScale);
}
