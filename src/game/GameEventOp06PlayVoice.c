// bdc 0x088ebe08 GameEventOp06PlayVoice
#include "bdc.h"

/* Handler of event opcode 0x06 (`GameEventExecCommand`): unless skipping, plays voice line
   `table[value]` (21 voice ids copied from `g_gameEventVoiceIds`, `SndBgmPlayVoice`). */

void GameEventOp06PlayVoice(GameEvent *self, s16 value, s16 unused)

{
  s32 ids[21];
  u32 idx = (u16)value;

  if ((self->flags & 1) == 0) {
    memcpy(ids, g_gameEventVoiceIds, 0x54);
    if (idx < 0x15) {
      SndBgmPlayVoice(ids[idx]);
    }
  }
}
