// bdc 0x088f1240 GameEventOp8DSetFlag
#include "bdc.h"

/* Handler of event opcode 0x8d (`GameEvent470ExecCommand`): sets story flag `id`
   (`GameEventFlagSet`) and, outside flag range 4, calls `GameEventFlagIs3d1(id)`. */

void GameEventOp8DSetFlag(GameEvent *self, u16 id, s16 arg)

{
  GameEventFlagSet(id);
  if (GameEventFlagInRange(4, id) == 0) {
    GameEventFlagIs3d1(id);
  }
  return;
}
