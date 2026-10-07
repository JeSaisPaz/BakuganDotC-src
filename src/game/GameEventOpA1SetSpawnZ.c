// bdc 0x088f1700 GameEventOpA1SetSpawnZ
#include "bdc.h"

/* Handler of event opcode 0xA1 (`GameEvent470ExecCommand`): sets the spawn z (s32 at `0x08b00d24`, word 2 of
   `g_gameUnlockFlags`) to `((s64)(value << 12) << 12) / g_gameFixedScale`: the 16-bit `value` is shifted by 12
   in 32 bits, sign-extended, shifted by 12 again in 64 bits, then divided with `__divdi3`. */

void GameEventOpA1SetSpawnZ(GameEvent *self, s16 value, s16 unused)

{
  s32 fixed;
  s64 scaled;

  fixed = (s32)value << 12;
  scaled = (s64)fixed << 12;
  ((s32 *)g_gameUnlockFlags)[2] = (s32)__divdi3(scaled, g_gameFixedScale);
}
