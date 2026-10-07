// bdc 0x088ebd98 GameEventOp05StopSe
#include "bdc.h"

/* Handler of event opcode 0x05 (`GameEventExecCommand`): stops the sound effect mapped to `arg`
   by the `{id, soundId}` table (same table as `GameEventOp04PlaySe`), scanned downward from
   `0x08a99078`; does nothing if the id is not found or there is no sound manager. */

void GameEventOp05StopSe(GameEvent *self, u16 arg, s16 unused)

{
  s32 n = 0x14;
  const u32 *e = &g_gameEventVoiceIds[18];

  do {
    n--;
    if (e[0] == arg) {
      if (!SndHasManager()) {
        return;
      }
      SndManagerStop(SndGetManager(), e[1]);
      return;
    }
    e -= 2;
  } while (n >= 0);
  return;
}
