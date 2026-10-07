// bdc 0x088eddc0 GameEventOp42PropPlayMotion
#include "bdc.h"

/* Handler of event opcode 0x42 (`GameEventExecCommand`): plays the selected motion `+0x262` on
   the prop of slot `flag` (`GameEventPropPlayMotion`, loop unless bit 3 of `+0x273`, speed
   `arg`). */

void GameEventOp42PropPlayMotion(GameEvent *self, u8 flag, s16 arg)

{
  void *prop;
  
  prop = self->props[flag].prop;
  if (prop != (void *)0x0) {
    GameEventPropPlayMotion(prop,self->propMotion,(self->flags & 8) == 0,(u8)arg);
  }
  return;
}

