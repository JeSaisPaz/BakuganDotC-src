// bdc 0x088f26bc GameEvent470SkipLabels
#include "bdc.h"

/* Vtable slot 10 of the task-470 event: skips `count` times to the next opcode 0xb2
   (`GameEventSkipToOpcode`). */

void GameEvent470SkipLabels(GameEvent470 *self, s32 count)

{
  s32 i;
  
  i = 0;
  if (0 < count) {
    do {
      GameEventSkipToOpcode(&self->base,0xb2);
      i = i + 1;
    } while (i < count);
  }
  return;
}

