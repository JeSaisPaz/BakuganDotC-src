// bdc 0x088f1760 GameEventOpA2SetSpawnAngle
#include "bdc.h"

/* Handler of event opcode 0xa2 (`GameEvent470ExecCommand`): sets the spawn heading
   (16-bit angle at `0x08b00d28`, `g_gameEventState`+0x178) to `arg` degrees. */

void GameEventOpA2SetSpawnAngle(GameEvent *self, u8 flag, s16 arg)

{
  ((u16 *)&g_gameEventState)[0xbc] = (u16)(s32)((float)(s32)arg * 65536.0f * 0.0027777778f);
}
