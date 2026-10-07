// bdc 0x088ed3a4 GameEventOp2BPropDelete
#include "bdc.h"

/* Handler of event opcode 0x2b (`GameEventExecCommand`): releases and deletes the prop of slot
   `flag` (`GameEventPropReleaseModel`, `GameEventPropDelete`). */

void GameEventOp2BPropDelete(GameEvent *self, u8 flag, s16 arg)

{
  void *prop;
  
  prop = self->props[flag].prop;
  if (prop != (void *)0x0) {
    GameEventPropReleaseModel(prop);
    GameEventPropDelete(self->props[flag].prop);
    self->props[flag].prop = (void *)0x0;
  }
  return;
}

