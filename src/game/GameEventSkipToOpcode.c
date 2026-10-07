// bdc 0x088eb67c GameEventSkipToOpcode
#include "bdc.h"

/* Advances the command pointer `+0x20` (index `+0x250`) one command at a time until a command with
   opcode `op` is reached or the end (`+0x252`). */

void GameEventSkipToOpcode(GameEvent *self, u8 op)

{
  GameEventCommand *cur;
  
  self->cmdIndex = self->cmdIndex + 1;
  self->cmd = self->cmd + 1;
  cur = self->cmd;
  while( true ) {
    if (cur->op == op) {
      return;
    }
    self->cmdIndex = self->cmdIndex + 1;
    self->cmd = cur + 1;
    if (self->cmdEnd <= self->cmdIndex) break;
    cur = self->cmd;
  }
  return;
}

