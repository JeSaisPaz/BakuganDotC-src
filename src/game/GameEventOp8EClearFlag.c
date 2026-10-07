// bdc 0x088f1284 GameEventOp8EClearFlag
#include "bdc.h"

/* Handler of event opcode 0x8e (`GameEvent470ExecCommand`): clears story flag `id`
   (`GameEventFlagClear`) and, outside flag range 4, calls `GameEventFlagIs3d1(id)`
   (result unused). `arg` is unused. */

void GameEventOp8EClearFlag(GameEvent *self, u16 id, s16 arg)
{
  GameEventFlagClear(id);
  if (GameEventFlagInRange(4, id) == 0) {
    GameEventFlagIs3d1(id);
  }
}
