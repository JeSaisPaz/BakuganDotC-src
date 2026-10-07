// bdc 0x088f1640 GameEventOp9FSetSpawnX
#include "bdc.h"

/* Handler of event opcode 0x9F (`GameEvent470ExecCommand`): sets the spawn x (s32 at `0x08b00d1c`, word 0 of
   `g_gameUnlockFlags`) to ((s64)(value << 12) << 12) / g_gameFixedScale, i.e. `value` converted to 20.12
   fixed point and then scaled up by another 12 bits before the 64-bit division. */

void GameEventOp9FSetSpawnX(GameEvent *self, s16 value, s16 unused)
{
  s64 scaled;

  scaled = (s64)((s32)value << 12) << 12;
  ((s32 *)g_gameUnlockFlags)[0] = (s32)__divdi3(scaled, g_gameFixedScale);
}
