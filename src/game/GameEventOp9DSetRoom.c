// bdc 0x088f151c GameEventOp9DSetRoom
#include "bdc.h"

/* Handler of event opcode 0x9d (`GameEvent470ExecCommand`): sets both room bytes
   `0x08b00bd5`/`0x08b00bd6` to `flag`. */

void GameEventOp9DSetRoom(GameEvent *self, u8 flag, s16 arg)

{
  g_gameEventFlags[1] = flag;
  g_gameEventFlags[2] = flag;
  return;
}

