// bdc 0x088f1640 GameEventOp9FSetSpawnX
#include "bdc.h"

/* Handler of event opcode 0x9F (`GameEvent470ExecCommand`): sets the spawn x,
   `g_gameEventState`.spawn.pos[0] (0x08b00bb0+0x16c), to ((s64)(value << 12) << 12) / g_gameFixedScale,
   i.e. `value` converted to 20.12 fixed point and then scaled up by another 12 bits before the 64-bit
   division. Returns nothing. */

void GameEventOp9FSetSpawnX(GameEvent *self, s16 value, s16 unused)
{
  s64 scaled;

  scaled = (s64)((s32)value << 12) << 12;
  g_gameEventState.spawn.pos[0] = (s32)__divdi3(scaled, g_gameFixedScale);
}
