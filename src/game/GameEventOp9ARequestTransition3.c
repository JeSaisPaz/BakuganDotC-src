// bdc 0x088f1440 GameEventOp9ARequestTransition3
#include "bdc.h"

/* Handler of event opcode 0x9a (`GameEvent470ExecCommand`): requests field transition kind 3 and
   sets bit 0 of `0x08b00dc7` from `flag`. */

void GameEventOp9ARequestTransition3(GameEvent *self, u8 flag, s16 arg)

{
  if ((self->flags & 1) != 0) {
    self->flags = self->flags & 0xfb;
  }
  g_gameEventTransitionKind = '\x03';
  g_gameEventTransitionFlags = g_gameEventTransitionFlags & 0xfe | flag & 1;
  return;
}

