// bdc 0x088f19f4 GameEventOpB0SetFieldFlagD18
#include "bdc.h"

/* Handler of event opcode 0xb0 (`GameEvent470ExecCommand`): sets bit 0 of `0x08b00d18` from
   `flag`. */

void GameEventOpB0SetFieldFlagD18(GameEvent *self, u8 flag, s16 arg)

{
  g_gameEventFieldFlags = (g_gameEventFieldFlags & 0xfe) | (flag & 1);
  return;
}

